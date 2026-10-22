/*
 * A006932  Number of permutations of [n] with at least one strong fixed point
 *          (a permutation p of {1,2,...,n} is said to have j as a strong
 *          fixed point if p(k) < j for k < j and p(k) > j for k > j).
 * https://oeis.org/A006932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} (n-k)! * A052186(k-1), where A052186(k) = k!
 *            - a(k), a(0) = 0 (so A052186(0) = 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *fact_tab = 0;
static seq_t *a_tab = 0;
static seq_t *c_tab = 0;
static idx_t cap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
	if (n >= cap) {
		idx_t newcap = n + 1;
		if (newcap < 2 * cap) newcap = 2 * cap;
		if (newcap < 64) newcap = 64;
		fact_tab = grow_zero(fact_tab, cap, newcap);
		a_tab = grow_zero(a_tab, cap, newcap);
		c_tab = grow_zero(c_tab, cap, newcap);
		cap = newcap;
	}
	while (done <= n) {
		idx_t m = done;
		if (m == 0) {
			fact_tab[0] = 1;
			a_tab[0] = 0;
			c_tab[0] = 1;
		} else {
			fact_tab[m] = fact_tab[m - 1] * (seq_t)m;
			seq_t s = 0;
			for (idx_t k = 1; k <= m; k++) {
				s += c_tab[k - 1] * fact_tab[m - k];
			}
			a_tab[m] = s;
			c_tab[m] = fact_tab[m] - s;
		}
		done++;
	}
	return a_tab[n];
}
