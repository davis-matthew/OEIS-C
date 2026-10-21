/*
 * A213173  a(n) = 4^floor(n/2), Powers of 4 repeated.
 * https://oeis.org/A213173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^floor(n/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4*a(n-2) with a(0) = a(1) = 1 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    return pow4(n / 2);
}
