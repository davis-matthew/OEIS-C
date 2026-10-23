/*
 * A127960  a(n) = n^2*3^n.
 * https://oeis.org/A127960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* Compute n^2 in seq_t to avoid overflow */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Final result */
    return n_sq * pow3;
}
