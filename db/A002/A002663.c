/*
 * A002663  a(n) = 2^n - C(n,0) - C(n,1) - C(n,2) - C(n,3).
 * https://oeis.org/A002663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + C(n-1,3), a(0) = 0; C(m,3) and C(m,2) updated
 *            by additions only, so no intermediate needs to be larger than
 *            seq_t
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t r = 0;   /* a(m) */
	seq_t c3 = 0;  /* C(m,3) */
	seq_t c2 = 0;  /* C(m,2) */
	idx_t m;
	for (m = 0; m < n; m++) {
		r = r * 2 + c3;      /* a(m+1) = 2*a(m) + C(m,3) */
		c3 += c2;            /* C(m+1,3) = C(m,3) + C(m,2) */
		c2 += (seq_t)m;      /* C(m+1,2) = C(m,2) + m */
	}
	return r;
}
