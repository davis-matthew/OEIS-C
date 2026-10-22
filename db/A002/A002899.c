/*
 * A002899  Number of n-step polygons on f.c.c. lattice.
 * https://oeis.org/A002899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, nice
 * Formula:   a(n) = Sum_{k=0..n, k even} binomial(n,k)*binomial(k,k/2) *
 *            Sum_{j=0..k} binomial(k,j)*c(j+n-k)*c(n-j), where c(m) =
 *            binomial(m,m/2) for even m and 0 for odd m (constant term of
 *            (uv+vw+wu)^n, u=x+1/x, ...). Arbitrary-precision builds use
 *            instead the recurrence n^3*a(n) = 2*n*(2*n-1)*(n-1)*a(n-1)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   a002899_fixed, a002899_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a002899_fixed.h"
#include "../utils/a002899_sum.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
	idx_t i;
	if (n == 0) return 1;
	if (n == 1) return 0;
	if (n == 2) return 12;

	if (a002899_fixed()) return a002899_sum(n);

	if (memo == NULL) {
		memo_cap = (n > 100) ? n + 1 : 100;
		memo = (seq_t *)malloc(sizeof(seq_t) * memo_cap);
		memo[0] = 1;
		memo[1] = 0;
		memo[2] = 12;
		memo_len = 3;
	} else if (n >= memo_cap) {
		idx_t new_cap = (n + 1) * 2;
		seq_t *nm = (seq_t *)malloc(sizeof(seq_t) * new_cap);
		for (i = 0; i < memo_len; i++) nm[i] = memo[i];
		free(memo);
		memo = nm;
		memo_cap = new_cap;
	}

	for (i = memo_len; i <= n; i++) {
		seq_t sn = (seq_t)i;
		seq_t n1 = sn - 1;
		seq_t n2 = sn - 2;
		seq_t num = 2 * sn * (2 * sn - 1) * n1 * memo[i - 1]
		          + 16 * n1 * (5 * sn * sn - 10 * sn + 6) * memo[i - 2]
		          + 96 * n1 * n2 * (2 * sn - 3) * memo[i - 3];
		memo[i] = num / (sn * sn * sn);
		memo_len = i + 1;
	}
	return memo[n];
}
