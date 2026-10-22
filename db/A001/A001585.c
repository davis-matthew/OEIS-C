/*
 * A001585  a(n) = 3^n + n^3.
 * https://oeis.org/A001585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + n^3
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
	seq_t m = (seq_t)n;
	return pow3(n) + m * m * m;
}
