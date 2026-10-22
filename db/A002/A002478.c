/*
 * A002478  Bisection of A000930.
 * https://oeis.org/A002478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3), a(0) = 1, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 0) return (seq_t)1;
	if (n == 1) return (seq_t)1;
	if (n == 2) return (seq_t)3;
	
	seq_t a0 = (seq_t)1;
	seq_t a1 = (seq_t)1;
	seq_t a2 = (seq_t)3;
	
	for (idx_t i = 3; i <= n; i++) {
		seq_t a3 = a2 + (seq_t)2 * a1 + a0;
		a0 = a1;
		a1 = a2;
		a2 = a3;
	}
	
	return a2;
}
