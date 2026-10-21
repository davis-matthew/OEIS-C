/*
 * A218257  a(n) = |{m : multiplicative order of n mod m = 8}|.
 * https://oeis.org/A218257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n^8-1) - tau(n^4-1) (tau = number of divisors)
 * Range:     u8 n <= 26, u16 n <= 3276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mulmod_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod_uint128.h"

/* ------------------------------------------------------------------ */
/* ------------------------- Random → xorshift -----------------------*/
/* ------------------------------------------------------------------ */
static uint64_t _seed_xorshift = 88172645463325252ULL;
static uint64_t rand64(void)
{
    uint64_t x = _seed_xorshift;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    _seed_xorshift = x;
    return x;
}

/* ------------------------------------------------------------------ */
/* --------------------- modular arithmetic (128-bit) ----------------*/

static unsigned __int128 powmod_uint128(unsigned __int128 a,
                                         unsigned __int128 e,
                                         unsigned __int128 mod)
{
    unsigned __int128 res = 1;
    while (e) {
        if (e & 1)
            res = mulmod_uint128(res, a, mod);
        a = mulmod_uint128(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* ---------------------------- Miller-Rabin ------------------------*/
/* ------------------------------------------------------------------ */
static bool is_prime_uint128(unsigned __int128 n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    /* Small prime divisors up to 100 */
    const uint64_t small_primes[] = {5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
                                    67,71,73,79,83,89,97};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    /* Write n-1 as d*2^s */
    unsigned __int128 d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Deterministic bases for < 2^128 */
    const unsigned __int128 bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        unsigned __int128 a = bases[i];
        if (a % n == 0) continue;
        unsigned __int128 x = powmod_uint128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod_uint128(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* --------------- gcd for unsigned 128-bit integers ----------------*/
/* ------------------------------------------------------------------ */
static unsigned __int128 gcd_uint128(unsigned __int128 a,
                                      unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* ---------------------------- Pollard Rho ------------------------*/
/* ------------------------------------------------------------------ */
static unsigned __int128 pollard_rho_uint128(unsigned __int128 n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (1) {
        unsigned __int128 c = rand64() % (n - 2) + 2;
        unsigned __int128 x = rand64() % (n - 2) + 2;
        unsigned __int128 y = x;
        unsigned __int128 d = 1;
        while (d == 1) {
            x = (mulmod_uint128(x, x, n) + c) % n;
            y = (mulmod_uint128(y, y, n) + c) % n;
            y = (mulmod_uint128(y, y, n) + c) % n;
            unsigned __int128 diff = x > y ? x - y : y - x;
            d = gcd_uint128(diff, n);
            if (d == n) break;
        }
        if (d > 1 && d < n) return d;
    }
}

/* ------------------------------------------------------------------ */
/* ---------------------- recursive factorization -------------------*/
/* ------------------------------------------------------------------ */
static void factor_uint128_recursive(unsigned __int128 n,
                                     unsigned __int128 *factors,
                                     size_t *cnt)
{
    if (n == 1) return;
    if (is_prime_uint128(n)) {
        factors[(*cnt)++] = n;
        return;
    }
    unsigned __int128 d = pollard_rho_uint128(n);
    factor_uint128_recursive(d, factors, cnt);
    factor_uint128_recursive(n / d, factors, cnt);
}

/* ------------------------------------------------------------------ */
/* --------------------------- divisor count ------------------------*/
/* ------------------------------------------------------------------ */
static seq_t tau_uint128(unsigned __int128 n)
{
    if (n <= 1) return 1;                /* tau(1)=1, tau(0)=0 not used here */
    /* maximum number of prime factors is small (< 64), allocate buffer */
    unsigned __int128 factors[64];
    size_t fcnt = 0;
    factor_uint128_recursive(n, factors, &fcnt);

    /* sort factors */
    for (size_t i = 1; i < fcnt; ++i) {
        unsigned __int128 key = factors[i];
        size_t j = i;
        while (j > 0 && factors[j-1] > key) {
            factors[j] = factors[j-1];
            --j;
        }
        factors[j] = key;
    }

    /* compute tau from multiplicities */
    unsigned long long tau = 1;
    size_t i = 0;
    while (i < fcnt) {
        unsigned __int128 p = factors[i];
        size_t exp = 0;
        while (i < fcnt && factors[i] == p) {
            ++exp;
            ++i;
        }
        tau *= (exp + 1);
    }
    return (seq_t)tau;
}

/* ------------------------------------------------------------------ */
/* ------------------------------ main --------------------------------*/
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Not used, but safe */
    unsigned __int128 n128 = n;
    unsigned __int128 n2   = n128 * n128;
    unsigned __int128 n4   = n2   * n2;
    unsigned __int128 n8   = n4   * n4;
    unsigned __int128 val8 = n8 - 1;
    unsigned __int128 val4 = n4 - 1;

    seq_t tau8 = tau_uint128(val8);
    seq_t tau4 = tau_uint128(val4);
    return tau8 - tau4;
}
