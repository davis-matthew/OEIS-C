/*
 * A002893  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k).
 * https://oeis.org/A002893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k) {
	if (k > n) return 0;
	if (k == 0) return 1;
	if (k > n - k) k = n - k;
	
	seq_t result = 1;
	for (idx_t i = 1; i <= k; i++) {
		result = result * (seq_t)(n - i + 1) / (seq_t)i;
	}
	return result;
}

seq_t a(idx_t n) {
	seq_t sum = 0;
	for (idx_t k = 0; k <= n; k++) {
		seq_t binom_n_k = binom(n, k);
		seq_t binom_2k_k = binom(2 * k, k);
		seq_t term = binom_n_k * binom_n_k * binom_2k_k;
		sum = sum + term;
	}
	return sum;
}
