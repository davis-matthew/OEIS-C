/*
 * A013735  a(n) = 4^(3*n+2).
 * https://oeis.org/A013735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(3*n+2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent 3*n + 2.  idx_t is unsigned, *and* large enough for the values
       we need to support (in practice n <= 10000). */
    idx_t exp = 3 * n + 2;
    /* Use the generic power routine from the utilities. */
    return seq_pow((seq_t)4, exp);
}
