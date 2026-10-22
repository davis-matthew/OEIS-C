/*
 * A001967  u-pile positions for the 4-Wythoff game with i=3.
 * https://oeis.org/A001967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n+3/4)*(sqrt(5)-1) ) = floor( (isqrt(5*(4n+3)^2)
 *            - (4n+3)) / 4 )
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001967_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001967_isqrt128.h"

seq_t a(idx_t n)
{
	unsigned __int128 m = (unsigned __int128)4 * n + 3;
	unsigned __int128 s = a001967_isqrt128(5 * m * m);
	unsigned __int128 v = (s - m) / 4;
	seq_t hi = (seq_t)(uint64_t)(v >> 64);
	seq_t lo = (seq_t)(uint64_t)v;
	if (hi == 0)
		return lo;
	return (hi << 32 << 32) + lo;
}
