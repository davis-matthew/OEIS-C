/*
 * A221906  a(n) = 4^n + 4*n.
 * https://oeis.org/A221906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 4*n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow4(n);           /* 4^n */
    seq_t four_times = (seq_t)4 * n;    /* 4*n, cast to seq_t */
    return pow_val + four_times;
}
