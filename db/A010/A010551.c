/*
 * A010551  Multiply successively by 1,1,2,2,3,3,4,4,..., n >= 1, a(0) = 1.
 * https://oeis.org/A010551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ⌊n/2⌋! · ⌊(n+1)/2⌋!
 * (OEIS A010551 – multiply successively by 1,1,2,2,3,3,…)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Compute the two factorials */
    idx_t fa = n / 2;             /* floor(n/2)   */
    idx_t fb = (n + 1) / 2;       /* floor((n+1)/2) */

    seq_t fact_a = (seq_t)1;
    for (idx_t i = 2; i <= fa; ++i) {
        fact_a *= (seq_t)i;       /* multiply by i */
    }

    seq_t fact_b = (seq_t)1;
    for (idx_t i = 2; i <= fb; ++i) {
        fact_b *= (seq_t)i;
    }

    return fact_a * fact_b;
}
