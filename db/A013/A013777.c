/*
 * A013777  a(n) = 2^(4*n + 3).
 * https://oeis.org/A013777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n + 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The exponent is 4*n + 3.  We compute the power of two
       using the provided pow2 helper, which returns 2^exp
       as a seq_t value. */
    idx_t exp = n * 4 + 3;
    return pow2(exp);
}
