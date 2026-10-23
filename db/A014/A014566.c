/*
 * A014566  Sierpiński numbers of the first kind: n^n + 1.
 * https://oeis.org/A014566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    return pow_int((seq_t)n, n) + 1;
}
