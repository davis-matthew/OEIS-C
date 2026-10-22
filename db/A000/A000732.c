/*
 * A000732  Boustrophedon transform of 1 & primes: 1,2,3,5,7,...
 * https://oeis.org/A000732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,n) of the boustrophedon (Seidel-Entringer) triangle:
 *            T(n,0) = b(n), T(n,k) = T(n,k-1) + T(n-1,n-k) for 1 <= k <= n,
 *            where b(0) = 1 and b(k) = prime(k) for k >= 1 (sequence A008578
 *            shifted)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow, b_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"
#include "../utils/b_isprime.h"

static uint64_t *b_tab = NULL;
static idx_t b_len = 0, b_cap = 0;

/* b(m): 1 for m = 0, prime(m) otherwise */
static uint64_t b_term(idx_t m) {
	if (b_len == 0) {
		b_cap = 1024;
		b_tab = (uint64_t *)malloc(sizeof(uint64_t) * b_cap);
		b_tab[0] = 1;
		b_tab[1] = 2;
		b_len = 2;
	}
	while (b_len <= m) {
		if (b_len == b_cap) {
			b_cap *= 2;
			b_tab = (uint64_t *)realloc(b_tab, sizeof(uint64_t) * b_cap);
		}
		uint64_t c = b_tab[b_len - 1] + 1;
		while (!b_isprime(c)) c++;
		b_tab[b_len++] = c;
	}
	return b_tab[m];
}

static seq_t *vals = NULL;  /* vals[m] = a(m) */
static seq_t *rowA = NULL;  /* current row T(done-1, .) */
static seq_t *rowB = NULL;  /* scratch row */
static idx_t done = 0, cap = 0;

static void ensure(idx_t n) {
	if (n < done) return;
	if (cap < n + 1) {
		idx_t nc = cap * 2;
		if (nc < n + 1) nc = n + 1;
		if (nc < 64) nc = 64;
		if (vals == NULL) {
			vals = a306_alloc0(nc);
			rowA = a306_alloc0(nc);
			rowB = a306_alloc0(nc);
		} else {
			vals = a306_grow(vals, cap, nc);
			rowA = a306_grow(rowA, cap, nc);
			rowB = a306_grow(rowB, cap, nc);
		}
		cap = nc;
	}
	for (idx_t m = done; m <= n; m++) {
		if (m == 0) {
			rowA[0] = 1;
		} else {
			rowB[0] = (seq_t)b_term(m);
			for (idx_t k = 1; k <= m; k++)
				rowB[k] = rowB[k - 1] + rowA[m - k];
			seq_t *t = rowA;
			rowA = rowB;
			rowB = t;
		}
		vals[m] = rowA[m];
	}
	done = n + 1;
}

seq_t a(idx_t n) {
	ensure(n);
	return vals[n];
}
