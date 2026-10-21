/*
 * A214081  a(n) = floor( n^(1/3) )!.
 * https://oeis.org/A214081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n^(1/3))!
 * Range:     u8 n <= 215, u16 n <= 728, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    idx_t k = int_cube_root(n);
    seq_t res = 1;
    for (idx_t i = 2; i <= k; ++i)
    {
        res *= (seq_t)i;
    }
    return res;
}
