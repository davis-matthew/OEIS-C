/*
 * A024039  a(n) = 4^n - n^3.
 * https://oeis.org/A024039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n^3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow4 = pow_uint(4, n);
    seq_t n_seq = (seq_t)n;
    seq_t n_cube = n_seq * n_seq * n_seq;
    return pow4 - n_cube;
}
