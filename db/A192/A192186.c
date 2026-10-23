/*
 * A192186  a(n) = binomial(2*n, floor(n*sqrt(2))).
 * https://oeis.org/A192186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, floor(n*sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0) is the first term listed) */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute binomial( n , k ) with k <= n/2, using gcd reduction
   to keep intermediate values small.  All arithmetic is done with
   the seq_t type (built‑in or arbitrary‑precision). */
static seq_t binom_reduced(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    if (k == 0) return (seq_t)1;

    seq_t result = (seq_t)1;          /* current product */

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);   /* numerator part   */
        seq_t den = (seq_t)i;             /* denominator part */

        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;                    /* den should now be 1 */

        result = result * num;
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute prod = 2 * n * n   using the seq_t type. */
    seq_t prod = (seq_t)n * (seq_t)n * (seq_t)2;

    /* Find k = floor(sqrt(2*n*n)) by binary search in [0,2*n]. */
    idx_t low = 0;
    idx_t high = n << 1;          /* 2*n */

    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        seq_t mid_sq = (seq_t)mid * (seq_t)mid;

        if (mid_sq <= prod)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t k = low;                 /* floor(n * sqrt(2)) */

    /* Return binomial(2*n, k) */
    return binom_reduced(n + n, k);
}
