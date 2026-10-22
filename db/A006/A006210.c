/*
 * A006210  Generalized Fibonacci numbers D_{n,2}.
 * https://oeis.org/A006210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{d|n, n/d odd} mu(n/d)*A001333(d) - [n is a power of
 *            2]) / n, where A001333(d) = 2*A001333(d-1) + A001333(d-2),
 *            A001333(0) = 1, A001333(1) = 1. Values are kept as q*n + r (0 <=
 *            r < n), so the division by n is exact
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 55, u128 n <= 106,
 *            big any n
 * Helpers:   mu_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mu_u64.h"

typedef struct {
	seq_t q;
	unsigned __int128 r;
} qr_t;

static qr_t qr_small(uint64_t v, uint64_t n) {
	qr_t z;
	z.q = (seq_t)(v / n);
	z.r = (unsigned __int128)(v % n);
	return z;
}

static qr_t qr_add(qr_t x, qr_t y, uint64_t n) {
	qr_t z;
	z.q = x.q + y.q;
	z.r = x.r + y.r;
	if (z.r >= (unsigned __int128)n) {
		z.r -= n;
		z.q += 1;
	}
	return z;
}

static qr_t qr_sub(qr_t x, qr_t y, uint64_t n) {
	qr_t z;
	z.q = x.q - y.q;
	if (x.r >= y.r) {
		z.r = x.r - y.r;
	} else {
		z.r = x.r + (unsigned __int128)n - y.r;
		z.q -= 1;
	}
	return z;
}

seq_t a(idx_t n) {
	qr_t h0, h1, hn, P, N, res;
	uint64_t k;
	if (n < 1) return 0;
	h0 = qr_small(1, n); /* A001333(0) */
	h1 = qr_small(1, n); /* A001333(1) */
	P = qr_small(0, n);
	N = qr_small(0, n);
	for (k = 1; k <= n; k++) {
		if (n % k == 0 && ((n / k) & 1)) {
			int m = mu_u64(n / k);
			if (m == 1) P = qr_add(P, h1, n);
			else if (m == -1) N = qr_add(N, h1, n);
		}
		if (k < n) {
			hn = qr_add(qr_add(h1, h1, n), h0, n);
			h0 = h1;
			h1 = hn;
		}
	}
	if ((n & (n - 1)) == 0)
		N = qr_add(N, qr_small(1, n), n);
	res = qr_sub(P, N, n);
	return res.q;
}
