/*
 * A192978  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) a(0)=0,
 *            a(1)=1, a(2)=4, a(3)=12, a(4)=29
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function requested by the build. */
seq_t a(idx_t n)
{
	/* Static cache of computed values. */
	static seq_t *cache = NULL;
	static size_t cache_len = 0;     /* allocated length of cache   */
	static idx_t last_computed = 0;  /* highest index computed */

	/* Initial values for indices 0..4. */
	static const seq_t init_vals[5] = {0, 1, 4, 12, 29};

	/* Helper: ensure `cache` can store indices up to `n`. */
	if (cache == NULL) {
		cache_len = 8;  /* initial allocation size */
		cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
		if (!cache) abort();  /* cannot proceed without memory */
		for (idx_t i = 0; i < 5; ++i)
			cache[i] = init_vals[i];
		last_computed = 4;
	} else if (cache_len <= (size_t)n) {
		/* grow cache (double until big enough) */
		size_t new_len = cache_len;
		while (new_len <= (size_t)n) new_len <<= 1;
		seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
		if (!tmp) abort();
		cache = tmp;
		cache_len = new_len;
	}

	/* Compute missing terms up to `n`. */
	if (n > last_computed) {
		for (idx_t i = last_computed + 1; i <= n; ++i) {
			/* recurrence: a(i) = 4*a(i-1) - 5*a(i-2) + a(i-3) + 2*a(i-4) - a(i-5) */
			seq_t term1 = cache[i-1] * (seq_t)4;           /* 4*a(i-1) */
			seq_t term3 = cache[i-3];                     /*  a(i-3) */
			seq_t term4 = cache[i-4] * (seq_t)2;           /* 2*a(i-4) */
			seq_t sum1  = term1 + term3 + term4;          /* positive part */

			seq_t term2 = cache[i-2] * (seq_t)5;           /* 5*a(i-2) */
			seq_t term5 = cache[i-5];                     /*  a(i-5) */
			seq_t sum2  = term2 + term5;                  /* negative part */

			cache[i] = sum1 - sum2;
		}
		last_computed = n;
	}

	return cache[n];
}
