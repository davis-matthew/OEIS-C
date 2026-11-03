/*
 * A001784  Second-order reciprocal Stirling number (Fekete) a(n) = [[2n+3,
 *          n]]. The number of n-orbit permutations of a (2n+3)-set with at
 *          least 2 elements in each orbit. Also known as associated Stirling
 *          numbers of the first kind (e.g., Comtet).
 * https://oeis.org/A001784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n>0, a(n) = (67 + 75*n + 20*n^2)*(2*n+3)!/(405*2^n*(n-1)!) =
 *            (20*n^2+75*n+67) * n * (2*n+2) * (2*n+3)!! / 405, a(0) = 1. The
 *            405 is cancelled factor by factor (greedy gcd), so every partial
 *            product divides the final value and never overflows when a(n)
 *            fits
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static seq_t a1784_mulf(seq_t r, uint64_t f, uint64_t *d)
{
	uint64_t g = gcd_u64(f, *d);
	*d /= g;
	return r * (seq_t)(f / g);
}

seq_t a(idx_t n)
{
	if (n == 0) return 1;

	uint64_t d = 405;
	uint64_t nn = (uint64_t)n;
	seq_t r = 1;

	r = a1784_mulf(r, 20 * nn * nn + 75 * nn + 67, &d);
	r = a1784_mulf(r, nn, &d);
	r = a1784_mulf(r, 2 * nn + 2, &d);
	for (uint64_t k = 3; k <= 2 * nn + 3; k += 2)
		r = a1784_mulf(r, k, &d);

	return r;
}
