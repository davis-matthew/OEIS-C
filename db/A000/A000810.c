/*
 * A000810  Expansion of e.g.f. (sin x + cos x)/cos 3x.
 * https://oeis.org/A000810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   from e.g.f. A(x)*cos(3x) = sin(x) + cos(x): a(n) = c(n) +
 *            Sum_{k>=1, 2k<=n} (-1)^(k+1)*binomial(n,2k)*9^k*a(n-2k), where
 *            c(n) = 1 if n mod 4 in {0,1}, else -1; a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
	static seq_t *f = NULL;
	static seq_t *row = NULL;
	static idx_t cnt = 0, cap = 0;

	if (n >= cap) {
		idx_t nc = cap ? cap : 64;
		while (nc <= n) nc *= 2;
		f = (seq_t *)realloc(f, nc * sizeof(seq_t));
		row = (seq_t *)realloc(row, nc * sizeof(seq_t));
		memset((void *)(f + cap), 0, (nc - cap) * sizeof(seq_t));
		memset((void *)(row + cap), 0, (nc - cap) * sizeof(seq_t));
		cap = nc;
	}

	while (cnt <= n) {
		idx_t m = cnt;
		/* advance Pascal row from m-1 to m */
		if (m >= 2) {
			for (idx_t j = m - 1; j >= 1; j--) row[j] += row[j - 1];
		}
		row[m] = 1;
		row[0] = 1;

		seq_t pos = 0, neg = 0;
		if ((m & 3) < 2) pos = 1;
		else neg = 1;

		seq_t p = 1;
		for (idx_t k = 1; 2 * k <= m; k++) {
			p *= 9;
			seq_t t = row[2 * k] * p * f[m - 2 * k];
			if (k & 1) pos += t;
			else neg += t;
		}
		f[m] = pos - neg;
		cnt++;
	}
	return f[n];
}
