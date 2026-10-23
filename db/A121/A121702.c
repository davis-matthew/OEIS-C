/*
 * A121702  a(n)=2^(n^3-1).
 * https://oeis.org/A121702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n^3-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n=0 (outside the OEIS offset) return 0, otherwise compute. */
    if (n == 0)
        return 0;

    /* Compute exponent n^3 - 1.  n is idx_t (uint64_t). */
    idx_t exp = n * n * n - 1;
    return pow2(exp);
}
