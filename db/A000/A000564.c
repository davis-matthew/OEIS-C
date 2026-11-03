/*
 * A000564  Number of discordant permutations.
 * https://oeis.org/A000564
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^6*(20 + 231x + 411x^2 + 72x^3 + 29x^4 - 36x^5 + 4x^6 -
 *            2x^7)/(1-x)^7, i.e. a(n) = Sum_{k=0..7} c_k * binomial(n-k, 6)
 *            with c = (20,231,411,72,29,-36,4,-2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 30 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a564_c6(idx_t m) {
	seq_t r = 1;
	idx_t i;
	if (m < 6) return 0;
	for (i = 0; i < 6; i++) {
		r = r * (seq_t)(m - i);
		r = r / (seq_t)(i + 1);
	}
	return r;
}

static seq_t a564_term(idx_t n, idx_t k) {
	if (n < k + 6) return 0;
	return a564_c6(n - k);
}

seq_t a(idx_t n) {
	seq_t result = 0;

	result += a564_term(n, 0) * 20;
	result += a564_term(n, 1) * 231;
	result += a564_term(n, 2) * 411;
	result += a564_term(n, 3) * 72;
	result += a564_term(n, 4) * 29;
	result += a564_term(n, 6) * 4;
	result -= a564_term(n, 5) * 36;
	result -= a564_term(n, 7) * 2;

	return result;
}
