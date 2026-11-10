/*
 * A084191  a(n) = A084190(A084190(n)).
 * https://oeis.org/A084191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A084190(A084190(n)), where A084190(n) = lcm{d-1 : d>1,
 *            d|n}
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static void A084191_factor(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; ex[c] = 0;
            while (n % p == 0) { n /= p; ++ex[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; ++c; }
    *cnt = c;
}

static seq_t A084191_gcd(seq_t x, seq_t y)
{
    while (y) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void A084191_enum(uint64_t cur, int idx, const uint64_t *pr,
                         const uint64_t *ex, int cnt, seq_t *lcm)
{
    if (idx == cnt) {
        if (cur > 1) {
            seq_t dm = (seq_t)(cur - 1);
            seq_t g = A084191_gcd(*lcm, dm);
            *lcm = (*lcm / g) * dm;
        }
        return;
    }
    uint64_t p = pr[idx];
    uint64_t pw = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        A084191_enum(cur * pw, idx + 1, pr, ex, cnt, lcm);
        pw *= p;
    }
}

static seq_t A084190_a(uint64_t n)
{
    seq_t lcm_val = 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fc = 0;
    A084191_factor(n, primes, exps, &fc);
    A084191_enum(1ULL, 0, primes, exps, (int)fc, &lcm_val);
    return lcm_val;
}

seq_t a(idx_t n)
{
    seq_t inner = A084190_a((uint64_t)n);
    /* inner value is small enough to factor by trial division */
    uint64_t m = (uint64_t)inner;
    return A084190_a(m);
}
