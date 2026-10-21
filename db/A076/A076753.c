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
 * Helpers:   factor_u64, floor_sqrt, sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/sigma_k.h"
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

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
