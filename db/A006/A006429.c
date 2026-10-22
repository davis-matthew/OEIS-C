/*
 * A006429  Number of loopless tree-rooted planar maps with 4 vertices and n
 *          faces.
 * https://oeis.org/A006429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. x^2*(4 + 95x + 198x^2 - 735x^3 + 1100x^4 - 995x^5 + 590x^6
 *            - 224x^7 + 50x^8 - 5x^9)/(1-x)^10, i.e. a(n) = Sum_{j=0..9} p_j
 *            * binomial(n+7-j, 9), a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 27, u64 n <= 350, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t small_gcd(uint64_t x, uint64_t y)
{
	while (y) { uint64_t t = x % y; x = y; y = t; }
	return x;
}

/* exact binomial(m, k), intermediates never exceed the result */
static seq_t binom9(uint64_t m, uint64_t k)
{
	if (m < k) return (seq_t)0;
	seq_t r = (seq_t)1;
	for (uint64_t i = 1; i <= k; i++) {
		uint64_t t = m - k + i;
		uint64_t rm = (uint64_t)(r % (seq_t)i);
		uint64_t g = small_gcd(i, rm);   /* gcd(r, i) */
		uint64_t d = i / g;
		r = (r / (seq_t)g) * (seq_t)(t / d);
	}
	return r;
}

seq_t a(idx_t n)
{
	static const int64_t p[10] = {4, 95, 198, -735, 1100, -995, 590, -224, 50, -5};
	if (n < 2) return (seq_t)0;
	seq_t pos = (seq_t)0, neg = (seq_t)0;
	for (int j = 0; j < 10; j++) {
		uint64_t m = (uint64_t)n + 7 - (uint64_t)j;
		seq_t b = binom9(m, 9);
		if (p[j] > 0) pos += b * (seq_t)(uint64_t)p[j];
		else neg += b * (seq_t)(uint64_t)(-p[j]);
	}
	return pos - neg;
}
