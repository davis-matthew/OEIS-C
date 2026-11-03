/*
 * A001468  There are a(n) 2's between successive 1's.
 * https://oeis.org/A001468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)*tau) - floor(n*tau), tau = (1+sqrt(5))/2,
 *            where floor(m*tau) = floor((m + isqrt(5*m^2))/2) is computed
 *            exactly in integers
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 109 terms through n = 954 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

static unsigned __int128 a001468_beatty(unsigned __int128 m) {
	unsigned __int128 s = isqrt_u128(5 * m * m);
	return (m + s) / 2;
}

seq_t a(idx_t n) {
	unsigned __int128 m = (unsigned __int128)n;
	unsigned __int128 d = a001468_beatty(m + 1) - a001468_beatty(m);
	return (seq_t)(int)d;
}
