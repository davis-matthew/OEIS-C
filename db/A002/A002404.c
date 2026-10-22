/*
 * A002404  Coefficients for step-by-step integration.
 * https://oeis.org/A002404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (n+1) * lcm(1..n+2) * Integral_{u=0..1} P_n(u)
 *            du, where P_n(u) = u*(u+1)*...*(u+n-1)*(u+n+1). Equivalently
 *            a(n) = (-1)^n (n+1)! lcm(1..n+2) (g(n) + (n+1) g(n+1)), with g
 *            the Adams-Bashforth coefficients 1, 1/2, 5/12, 3/8, 251/720, ...
 *            Here a(n)/((n+1)! lcm(1..n+2)) is the coeffic...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   a002404_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002404_isprime.h"

seq_t a(idx_t n)
{
	long long N = (long long)n;
	long long deg = 0, i, m;
	uint64_t p, top = (uint64_t)n + 2;
	seq_t L = 1, sum = 0, res;
	seq_t *c = (seq_t *)calloc((size_t)(N + 3), sizeof(seq_t));

	/* coefficients of P_n(u) = prod_{i=0..n+1, i != n} (u + i) */
	c[0] = 1;
	for (i = 0; i <= N + 1; i++) {
		seq_t f;
		if (i == N) continue;
		f = (seq_t)i;
		c[deg + 1] = c[deg];
		for (m = deg; m >= 1; m--)
			c[m] = c[m - 1] + f * c[m];
		c[0] = f * c[0];
		deg++;
	}

	/* L = lcm(1..n+2) */
	for (p = 2; p <= top; p++) {
		if (a002404_isprime(p)) {
			uint64_t pk = p;
			while (pk * p <= top) pk *= p;
			L *= (seq_t)pk;
		}
	}

	/* L * integral = sum c_m * L/(m+1) */
	for (m = 0; m <= deg; m++)
		sum += c[m] * (L / (seq_t)(m + 1));

	free(c);
	res = sum * ((seq_t)n + 1);
	if (n & 1) res = -res;
	return res;
}
