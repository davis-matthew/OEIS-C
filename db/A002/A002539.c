/*
 * A002539  Eulerian numbers of the second kind: <<n+3, n>>.
 * https://oeis.org/A002539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = E2(n+3, n), second-order Eulerian numbers, computed along
 *            diagonals D_j(k) = E2(k+j, k): D_j(k) = (k+1)*D_{j-1}(k) +
 *            (k+2j-1)*D_j(k-1), D_j(0) = 1 (j >= 1), D_0(k) = [k == 0]; a(n)
 *            = D_3(n), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
	seq_t d1 = 1, d2 = 1, d3 = 1; /* D_1(0), D_2(0), D_3(0) */
	idx_t k;
	for (k = 1; k <= n; k++) {
		seq_t k1 = (seq_t)k + 1;
		/* D_0(k) = 0 for k >= 1 */
		d1 = ((seq_t)k + 1) * d1;
		d2 = k1 * d1 + ((seq_t)k + 3) * d2;
		d3 = k1 * d2 + ((seq_t)k + 5) * d3;
	}
	return d3;
}
