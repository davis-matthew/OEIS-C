/*
 * A056450  a(n) = (3*2^n - (-2)^n)/2.
 * https://oeis.org/A056450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^floor((n+1)/2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    idx_t k = (n + 1) / 2;   /* floor((n+1)/2) */
    return pow4(k);
}
