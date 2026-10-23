/*
 * A047171  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n, floor((n-1)/2)) - 1 (OEIS A047171)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_exact(idx_t n, idx_t k)
{
    /* Return C(n,k) as exact seq_t using gcd reduction to avoid overflow */
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i); /* numerator term */
        seq_t den = (seq_t)i;           /* denominator term */

        /* Reduce num and den by gcd */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        /* Further reduce result and den */
        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;

        /* At this point den should be 1 */
        result *= num;
        /* den == 1 is guaranteed, so no division needed */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* a(0) = 0 */
    idx_t m = (n - 1) / 2;                     /* floor((n-1)/2) */
    seq_t bin = binom_exact(n, m);             /* C(n, m) */
    return bin - 1;                            /* subtract the empty subset */
}
