/*
 * A002462  Coefficients of Legendre polynomials.
 * https://oeis.org/A002462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = binomial(2*n,n)^2 /
 *            2^(A000120(n)+1) = (odd part of binomial(2*n,n))^2 *
 *            2^(A000120(n)-1), where A000120(n) = number of 1 bits in n. Odd
 *            part built iteratively: o(k) = o(k-1)*(2k-1)/oddpart(k), o(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
	seq_t o = 1;
	idx_t k, ok, t;
	int pc = 0;
	seq_t r;

	if (n == 0)
		return (seq_t)1;

	for (k = 1; k <= n; k++) {
		ok = k;
		while ((ok & 1) == 0)
			ok >>= 1;
		o = o * (seq_t)(2 * k - 1);
		o = o / (seq_t)ok;
	}

	t = n;
	while (t) {
		pc += (int)(t & 1);
		t >>= 1;
	}

	r = o * o;
	for (k = 1; k < (idx_t)pc; k++)
		r = r * (seq_t)2;
	return r;
}
