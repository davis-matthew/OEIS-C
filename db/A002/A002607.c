/*
 * A002607  Glaisher's chi_8(n).
 * https://oeis.org/A002607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(2^e) = 16^e, a(p^e) =
 *            p^(4*e)*(1+(-1)^e)/2 if p == 3 (mod 4), a(p^e) = a(p)*a(p^(e-1))
 *            - p^8*a(p^(e-2)) if p == 1 (mod 4), where a(p) =
 *            2*Re((x+i*y)^8), p = x^2+y^2, x even
 * Range:     i8 n <= 3, i16 n <= 15, i32 n <= 193, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   a2607_ap, a2607_isqrt, a2607_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a2607_ap.h"
#include "../utils/a2607_isqrt.h"
#include "../utils/a2607_two_squares.h"

seq_t a(idx_t n) {
	seq_t result = 1;
	idx_t t = n;

	while (t % 2 == 0) {
		result *= 16;
		t /= 2;
	}

	for (idx_t p = 3; p * p <= t; p += 2) {
		int e = 0;
		while (t % p == 0) {
			e++;
			t /= p;
		}
		if (e == 0) continue;

		if (p % 4 == 3) {
			if (e % 2 == 1) return 0;
			for (int i = 0; i < 4 * e; i++) result *= (seq_t)p;
		} else {
			seq_t ap = a2607_ap(p);
			seq_t p8 = 1;
			for (int i = 0; i < 8; i++) p8 *= (seq_t)p;
			seq_t prev2 = 1, prev = ap;
			for (int k = 2; k <= e; k++) {
				seq_t cur = prev * ap - p8 * prev2;
				prev2 = prev;
				prev = cur;
			}
			result *= prev;
		}
	}

	if (t > 1) {
		if (t % 4 == 3) return 0;
		result *= a2607_ap(t);
	}

	return result;
}
