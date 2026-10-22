/*
 * A001380  Weight distribution of binary Golay code of length 24.
 * https://oeis.org/A001380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 4n in the extended binary
 *            Golay code, computed by enumerating all 2^12 codewords
 *            m(x)*g(x), g(x) = x^11+x^10+x^6+x^5+x^4+x^2+1, and appending an
 *            overall parity bit
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   a001380_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001380_pop.h"

seq_t a(idx_t n)
{
	static unsigned cnt[25];
	static int ready = 0;
	if (!ready) {
		const uint32_t g = 0xC75u; /* generator polynomial of the cyclic [23,12] Golay code */
		uint32_t m;
		int i;
		for (i = 0; i < 25; i++)
			cnt[i] = 0;
		for (m = 0; m < 4096u; m++) {
			uint32_t c = 0;
			int b, w;
			for (b = 0; b < 12; b++)
				if ((m >> b) & 1u)
					c ^= g << b;
			w = a001380_pop(c);
			w += w & 1; /* extend with parity bit */
			cnt[w]++;
		}
		ready = 1;
	}
	if (n > 6)
		return (seq_t)0;
	return (seq_t)cnt[4 * n];
}
