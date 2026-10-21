/*
 * A215646  a(n) = n * (11*n^2 + 6*n + 1) / 6.
 * https://oeis.org/A215646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (11*n^2 + 6*n + 1) / 6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    return n_t * ((seq_t)11 * n_t * n_t + (seq_t)6 * n_t + (seq_t)1) / 6;
}
