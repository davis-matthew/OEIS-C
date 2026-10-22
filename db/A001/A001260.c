/*
 * A001260  Number of permutations of length n with 4 consecutive ascending
 *          pairs.
 * https://oeis.org/A001260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n-5)*a(n) = (n-5)*(n-1)*a(n-1) + (n-2)*(n-1)*a(n-2),
 *            a(1)=a(2)=a(3)=a(4)=0, a(5)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n <= 4) return 0;
	if (n == 5) return 1;
	
	seq_t a_prev2 = 0;
	seq_t a_prev1 = 1;
	
	for (idx_t i = 6; i <= n; i++) {
		seq_t term1 = (seq_t)(i - 1) * a_prev1;
		seq_t term2_num = (seq_t)(i - 2) * (seq_t)(i - 1) * a_prev2;
		seq_t term2 = term2_num / (seq_t)(i - 5);
		seq_t a_curr = term1 + term2;
		
		a_prev2 = a_prev1;
		a_prev1 = a_curr;
	}
	
	return a_prev1;
}
