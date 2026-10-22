/*
 * A001112  A continued fraction.
 * https://oeis.org/A001112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = c(n-1)*a(n-1) + a(n-2) for n >= 2,
 *            where c(k) are the partial quotients of sqrt(162) = [12;
 *            (1,2,1,2,12,2,1,2,1,24)], so c(k) = period[(k-1) mod 10] for k
 *            >= 1 (convergent denominators)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int a001112_period[10] = {1, 2, 1, 2, 12, 2, 1, 2, 1, 24};

seq_t a(idx_t n)
{
	seq_t prev = 0, cur = 1, next;
	idx_t i;
	if (n == 0) return prev;
	for (i = 2; i <= n; i++) {
		int c = a001112_period[(i - 2) % 10];
		next = (seq_t)c * cur + prev;
		prev = cur;
		cur = next;
	}
	return cur;
}
