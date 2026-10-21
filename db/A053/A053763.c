/*
 * A053763  a(n) = 2^(n^2 - n).
 * https://oeis.org/A053763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2^(n^2 - n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the exponent n(n-1) using 128‑bit arithmetic to avoid
       overflow for moderate values of n.  The exponent will fit in
       idx_t for all n that fit into the build's bit width. */
    __int128 tmp = (__int128)n * n - n;
    idx_t exp = (idx_t)tmp;
    return pow_uint(2, exp);   /* returns 2^exp as seq_t */
}
