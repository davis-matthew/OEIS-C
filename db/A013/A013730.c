/*
 * A013730  a(n) = 2^(3*n+1).
 * https://oeis.org/A013730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* exponent e = 3*n + 1 */
    idx_t e = n * 3 + 1;
    /* compute 2^e */
    return pow_int((seq_t)2, e);
}
