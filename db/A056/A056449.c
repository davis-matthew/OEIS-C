/*
 * A056449  a(n) = 3^floor((n+1)/2).
 * https://oeis.org/A056449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^floor((n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* exponent e = floor((n+1)/2) */
    idx_t e = n >> 1;
    if (n & 1U) {
        e += 1U;
    }
    return pow3(e);
}
