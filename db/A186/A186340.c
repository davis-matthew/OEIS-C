/*
 * A186340  a(n) = 2^A001840(n).
 * https://oeis.org/A186340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor((n+1)(n+2)/6)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute exponent floor((n+1)(n+2)/6) using the index type */
    idx_t prod = (n + 1) * (n + 2);
    idx_t e = prod / 6;
    /* pow2 returns 2^e in the appropriate seq_t type */
    return pow2(e);
}
