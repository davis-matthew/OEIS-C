/*
 * A054973  Number of numbers whose divisors sum to n.
 * https://oeis.org/A054973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m such that the sum of divisors of m
 *            equals n
 * Range:     u8 n <= 1303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
	/* return 0 for n == 0 (outside the defined offset) */
	if (n == 0)
		return 0;

	static idx_t max_n = 0;              /* maximum n processed so far   */
	static seq_t *counts = NULL;         /* counts[1..max_n]               */

	if (n > max_n) {
		size_t cap = (size_t)n + 1;      /* allocate indices 0..n          */

		/* allocate and zero new arrays */
		seq_t *new_counts = seq_alloc_new_array(cap);
		seq_t *sigma      = seq_alloc_new_array(cap);
		for (size_t i = 0; i < cap; ++i) {
			new_counts[i] = 0;
			sigma[i]      = 0;
		}

		/* Compute divisor sums up to n */
		for (idx_t i = 1; i <= n; ++i) {
			for (idx_t j = i; j <= n; j += i) {
				sigma[j] += (seq_t)i;
			}
		}

		/* Count how many times each sigma value occurs (only up to n) */
		for (idx_t m = 1; m <= n; ++m) {
			seq_t s = sigma[m];
			if (s <= n)
				++new_counts[(size_t)s];
		}

		/* Free previous memory and store the new results */
		if (counts)
			seq_free_array(counts);
		seq_free_array(sigma);

		counts = new_counts;
		max_n  = n;
	}

	return counts[(size_t)n];
}
