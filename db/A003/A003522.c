/*
 * A003522  a(n) = Sum_{k=0..n} C(n-k,3k).
 * https://oeis.org/A003522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4), a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 76, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n <= 3)
		return 1;
	
	seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1;
	
	for (idx_t i = 4; i <= n; i++) {
		seq_t an = 3 * a3 - 3 * a2 + a1 + a0;
		a0 = a1;
		a1 = a2;
		a2 = a3;
		a3 = an;
	}
	
	return a3;
}
