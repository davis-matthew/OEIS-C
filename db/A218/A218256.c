/*
 * A218256  a(n) = |{m : multiplicative order of n mod m = 7}|.
 * https://oeis.org/A218256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n^7-1)-tau(n-1) where tau is the divisor counting
 *            function
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_uint128, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* utilities provided by the framework */
#include "../utils/divisor_count_u64.h"
#include "../utils/cmp_uint128.h"

/* Miller‑Rabin primality test for 128‑bit numbers */
static const uint64_t MR_BASES[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};

static unsigned __int128 mulmod128(unsigned __int128 a, unsigned __int128 b, unsigned __int128 mod)
{
    unsigned __int128 res = 0;
    while (b) {
        if (b & 1) {
            if (res >= mod - a) res = res - (mod - a);
            else res += a;
        }
        b >>= 1;
        if (b) {
            if (a >= mod - a) a = a - (mod - a);
            else a += a;
        }
    }
    return res;
}

static unsigned __int128 powmod128(unsigned __int128 base, unsigned __int128 exp, unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mulmod128(result, base, mod);
        base = mulmod128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool is_prime128(unsigned __int128 n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(MR_BASES)/sizeof(MR_BASES[0]); ++i) {
        uint64_t p = MR_BASES[i];
        if (n % p == 0) return n == p;
    }
    unsigned __int128 d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(MR_BASES)/sizeof(MR_BASES[0]); ++i) {
        uint64_t a64 = MR_BASES[i];
        if (a64 >= n) continue;
        unsigned __int128 a = a64;
        unsigned __int128 x = powmod128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod128(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Pollard‑Rho factorisation for 128‑bit numbers */
static unsigned __int128 pollard_rho(unsigned __int128 n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    static unsigned __int128 seed = 1;
    unsigned __int128 c = ++seed;
    while (true) {
        unsigned __int128 x = 2, y = 2, d = 1;
        while (d == 1) {
            // x = f(x)
            unsigned __int128 x2 = mulmod128(x, x, n);
            x = mulmod128(x2 + c, 1, n); /* (x2 + c) mod n */
            // y = f(f(y))
            unsigned __int128 t = mulmod128(y, y, n);
            t = mulmod128(t + c, 1, n);
            y = mulmod128(t, t, n);
            y = mulmod128(y + c, 1, n);
            unsigned __int128 diff = x > y ? x - y : y - x;
            d = gcd128(diff, n);
            if (d == n) break;
        }
        if (d > 1 && d < n) return d;
        ++c;
    }
}

/* Recursive factorisation into primes */
static void factor_uint128(unsigned __int128 n, unsigned __int128 *arr, size_t *cnt)
{
    if (n == 1) return;
    if (is_prime128(n)) { arr[(*cnt)++] = n; return; }
    unsigned __int128 d = pollard_rho(n);
    factor_uint128(d, arr, cnt);
    factor_uint128(n / d, arr, cnt);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;                /* a(1) = 0 */

    unsigned __int128 nn = n;
    unsigned __int128 p7 = 1;
    for (int i = 0; i < 7; ++i) p7 *= nn;       /* n^7 */
    unsigned __int128 num = p7 - 1;             /* n^7-1 */

    /* factor num */
    unsigned __int128 facs[256];
    size_t fac_cnt = 0;
    factor_uint128(num, facs, &fac_cnt);

    /* sort factors to count multiplicities */
    qsort(facs, fac_cnt, sizeof(unsigned __int128), cmp_uint128);

    /* compute tau(n^7-1) */
    seq_t tau_full = 1;
    for (size_t i = 0; i < fac_cnt; ) {
        size_t j = i + 1;
        while (j < fac_cnt && facs[j] == facs[i]) ++j;
        size_t exp = j - i;
        tau_full = tau_full * (seq_t)(exp + 1);
        i = j;
    }

    /* compute tau(n-1) using provided utility (fits in 64 bits) */
    seq_t tau_prev = divisor_count_u64((uint64_t)(n - 1));

    /* answer */
    return tau_full - tau_prev;
}
