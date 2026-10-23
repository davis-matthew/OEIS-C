/*
 * A024091  a(n) = 8^n - n^3.
 * https://oeis.org/A024091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - n^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute 8^n */
    seq_t pow_val = seq_pow((seq_t)8, n);
    /* compute n^3 */
    seq_t n_cube = (seq_t)n * (seq_t)n * (seq_t)n;
    /* return result */
    return pow_val - n_cube;
}
