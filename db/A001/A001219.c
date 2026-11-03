/*
 * A001219  Triangular numbers of form a(a+1)(a+2).
 * https://oeis.org/A001219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A000217(A097571(n)); computed by a search over m >= 0 for
 *            products m(m+1)(m+2) that are triangular (8N+1 is an odd
 *            square). The sequence is finite and complete: 7 terms, the
 *            largest at m = 636
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a1219_isqrt(uint64_t x) {
	uint64_t lo = 0, hi = 4294967295ULL, mid;
	if (hi > x) hi = x;
	while (lo < hi) {
		mid = lo + (hi - lo + 1) / 2;
		if (mid * mid <= x) lo = mid;
		else hi = mid - 1;
	}
	return lo;
}

static int a1219_is_tri(uint64_t N) {
	uint64_t d = 8 * N + 1;
	uint64_t r = a1219_isqrt(d);
	return r * r == d;
}

seq_t a(idx_t n) {
	static uint64_t cache[16];
	static idx_t cache_len = 0;
	static int initialized = 0;

	if (!initialized) {
		uint64_t m;
		for (m = 0; m <= 20000 && cache_len < 16; m++) {
			uint64_t prod = m * (m + 1) * (m + 2);
			if (a1219_is_tri(prod)) cache[cache_len++] = prod;
		}
		initialized = 1;
	}

	if (n >= 1 && n <= cache_len) return (seq_t)cache[n - 1];
	return (seq_t)0;
}
