/*
 * A004952  a(n) = round(n*phi^17), where phi is the golden ratio, A001622.
 * https://oeis.org/A004952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^17) = floor((3571*n + 1 +
 *            floor(sqrt(5*(1597*n)^2)))/2), using phi^17 = (L(17) +
 *            F(17)*sqrt(5))/2 with L(17) = 3571, F(17) = 1597
 * Range:     u8 n <= 0, u16 n <= 18, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 x)
{
	unsigned __int128 r, y;
	if (x < 2)
		return x;
	r = x;
	y = (r + 1) / 2;
	while (y < r) {
		r = y;
		y = (r + x / r) / 2;
	}
	return r;
}

seq_t a(idx_t n)
{
	unsigned __int128 nn = (unsigned __int128)n;
	unsigned __int128 f = nn * 1597u;
	unsigned __int128 s = isqrt_u128(f * f * 5u);
	unsigned __int128 v = (nn * 3571u + 1u + s) / 2u;
	return u128_to_seq(v);
}
