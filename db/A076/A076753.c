/*
 * A076753  a(n) = nearest integer to the standard deviation of the divisors
 *          of n.
 * https://oeis.org/A076753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( sqrt( (d * σ₂(n) - σ₁(n)²) / (d * (d-1)) ) ) where
 *            d = number of divisors of n, σ₁(n) = sum of divisors, σ₂(n) =
 *            sum of squares of divisors
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 5889, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9408 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* smallest integer type that holds n */
    uint64_t nn = (uint64_t)n;

    /* sum of divisors (σ₁) */
    seq_t S1 = sigma_of(nn);

    /* sum of squares of divisors (σ₂) */
    seq_t S2 = sigma_k(nn, 2);

    /* number of divisors d = ∏(exp_i + 1) */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);

    seq_t d = 1;
    for (size_t i = 0; i < pcnt; ++i)
        d = d * ((seq_t)exps[i] + 1);

    /* n with a single divisor has zero standard deviation */
    if (d <= 1)
        return 0;

    /* numerator of the variance: d * S2 - S1^2 */
    seq_t num = d * S2 - S1 * S1;
    if (num == 0)
        return 0;

    /* denominator of the variance: d * (d-1) */
    seq_t denom = d * (d - 1);

    /* binary search for the largest integer f such that
       f^2 * denom <= num
       i.e. floor( sqrt(num / denom) ) */
    seq_t top = floor_sqrt(num);
    seq_t low = 0;
    seq_t high = top + 1;
    while (low + 1 < high) {
        seq_t mid = (low + high) / 2;
        seq_t mid_sq = mid * mid;
        seq_t cmp  = mid_sq * denom;
        if (cmp <= num)
            low = mid;
        else
            high = mid;
    }
    seq_t f = low;

    /* Decide whether to round up to f+1 or stay at f.
       Compare 4*num with denom * (2*f+1)^2.  */
    seq_t left  = 4 * num;
    seq_t tmp   = 2 * f + 1;
    seq_t right = denom * tmp * tmp;

    return (left >= right) ? (f + 1) : f;
}
