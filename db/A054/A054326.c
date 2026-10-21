/*
 * A054326  Eighth unsigned column of Lanczos triangle A053125 (decreasing
 *          powers).
 * https://oeis.org/A054326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n+8, 7) (OEIS A054326)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute k = 2*n + 8 */
    seq_t k = (seq_t)n * (seq_t)2 + (seq_t)8;

    /* compute binomial(k, 7) multiplicatively */
    seq_t binom = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= (idx_t)7; ++i) {
        binom = binom * (k - (seq_t)7 + (seq_t)i) / (seq_t)i;
    }

    /* multiply by 4^n */
    seq_t result = binom;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        result *= (seq_t)4;
    }

    return result;
}
