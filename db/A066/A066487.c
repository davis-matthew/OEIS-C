/*
 * A066487  a(n) = min( x : x^4 + n^4 = 0 mod (x+n-1) ).
 * https://oeis.org/A066487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{ x : x^4 + n^4 ≡ 0 (mod x + n – 1) } Equivalently
 *            (from comments): a(n)+n-1 is the least divisor of (n-1)^4 + n^4
 *            that is not less than n. Return a(n) for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 216, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9592 (2026-11-01)
 * Helpers:   gcd_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/mulmod.h"

static void genDivisors(const uint64_t *fac, const int *exp,
                        int depth, uint64_t cur, uint64_t n_val,
                        uint64_t *min_d)
{
    if (cur >= n_val && cur < *min_d) *min_d = cur;
    if (depth == 0) return;
    uint64_t p = fac[depth - 1];
    int e = exp[depth - 1];
    uint64_t cur_pow = cur;
    for (int i = 0; i <= e; ++i) {
        genDivisors(fac, exp, depth - 1, cur_pow, n_val, min_d);
        cur_pow *= p;
    }
}

static uint64_t pollardRho(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void factorRec(uint64_t n, uint64_t *buf, int *pos)
{
    if (n == 1) return;
    if (isPrime64(n)) {
        buf[(*pos)++] = n;
        return;
    }
    uint64_t d = pollardRho(n);
    factorRec(d, buf, pos);
    factorRec(n / d, buf, pos);
}

static void factor(uint64_t n, uint64_t *buf, int *cnt)
{
    *cnt = 0;
    factorRec(n, buf, cnt);
}

seq_t a(idx_t n_idx)
{
    uint64_t n = (uint64_t)n_idx;
    /* Compute M = (n-1)^4 + n^4 using seq_t arithmetic. */
    seq_t m = (seq_t)(n - 1) * (seq_t)(n - 1) *
              (seq_t)(n - 1) * (seq_t)(n - 1) +
              (seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n;

    /* Work with 64‑bit representation for factoring (M fits). */
    uint64_t m64 = (uint64_t)m;

    /* If M is prime, answer is M - n + 1. */
    if (isPrime64(m64)) {
        seq_t res = (seq_t)(m64 - n + 1);
        return res;
    }

    /* Factor M. */
    uint64_t buf[64];
    int cnt;
    factor(m64, buf, &cnt);

    /* Collect distinct primes and their exponents. */
    uint64_t fac[64];
    int exp[64];
    int k = 0;
    for (int i = 0; i < cnt; ) {
        uint64_t p = buf[i];
        int e = 0;
        while (i < cnt && buf[i] == p) { ++e; ++i; }
        fac[k] = p;
        exp[k] = e;
        ++k;
    }

    /* Find smallest divisor >= n. */
    uint64_t min_d = m64; /* worst case. */
    genDivisors(fac, exp, k, 1, n, &min_d);

    seq_t res = (seq_t)(min_d - n + 1);
    return res;
}
