/*
 * A002733  Numbers k such that (k^2 + 1)/10 is prime.
 * https://oeis.org/A002733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k == 3,7 (mod 10) (needed for 10 | k^2+1), testing
 *            whether (k^2+1)/10 is prime
 * Range:     u8 n <= 25, u16 n <= 2744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9756 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
	idx_t count = 0;
	uint64_t k = 3;

	for (;;) {
		uint64_t p = (k * k + 1) / 10;
		if (is_prime_u64(p)) {
			count++;
			if (count == n)
				return (seq_t)k;
		}
		if (k % 10 == 3)
			k += 4;
		else
			k += 6;
	}
}
