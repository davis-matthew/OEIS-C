/*
 * A003709  E.g.f. cos(sin(x)) (even powers only).
 * https://oeis.org/A003709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Sum_{k=0..n} T(2n,2k), where T(m,j) = number of
 *            partitions of [m] into j odd blocks, T(m,j) = T(m-2,j-2) +
 *            j^2*T(m-2,j), T(0,0) = 1 (|a(n)| = partitions of [2n] into an
 *            even number of odd blocks)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *row = 0;      /* row[k] = T(2*rown, 2*k), k = 0..rown */
static idx_t rown = 0;
static idx_t rowlen = 0;
static seq_t *cache = 0;    /* cache[i] = |a(i)|, i = 0..cachelen-1 */
static idx_t cachelen = 0;

seq_t a(idx_t n) {
	if (row == 0) {
		row = grow_zero(0, 0, 1);
		rowlen = 1;
		row[0] = 1;
		rown = 0;
		cache = grow_zero(0, 0, 1);
		cachelen = 1;
		cache[0] = 1;
	}
	while (cachelen <= n) {
		idx_t m = rown + 1;
		row = grow_zero(row, rowlen, m + 1);
		rowlen = m + 1;
		for (idx_t k = m; k >= 1; k--) {
			seq_t t = row[k];
			t *= (seq_t)(2 * k);
			t *= (seq_t)(2 * k);
			row[k] = row[k - 1] + t;
		}
		row[0] = 0;
		rown = m;
		seq_t s = 0;
		for (idx_t k = 0; k <= m; k++) s += row[k];
		cache = grow_zero(cache, cachelen, cachelen + 1);
		cache[cachelen] = s;
		cachelen++;
	}
	seq_t r = cache[n];
	if (n & 1) r = -r;
	return r;
}
