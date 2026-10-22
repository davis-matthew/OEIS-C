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
 * Helpers:   a1219_is_tri, a1219_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1219_is_tri.h"
#include "../utils/a1219_isqrt.h"

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
