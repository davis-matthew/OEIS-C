/*
 * A030655  Pair up the numbers.
 * https://oeis.org/A030655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concat_parts(2*n-1, 2*n)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    idx_t first  = n * 2 - 1;
    idx_t second = n * 2;
    return concat_parts(first, second);
}
