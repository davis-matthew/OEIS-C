/*
 * A037798  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=0}^{n-1} (3-(k%4)) * 9^{n-1-k}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return a(n) for the OEIS sequence A037798.  */
seq_t a(idx_t n)
{
	/* Static cache of already computed values. */
	static seq_t *cache = NULL;      /* array of a(1) .. a(max_n) */
	static size_t cap = 0;           /* allocated capacity */
	static size_t max_n = 0;         /* largest n computed so far, 0 if none */

	if (n == 0) return 0;  /* undefined for offset 1, but safe fallback */

	size_t idx = (size_t)n;  /* safe as n will be small in tests */

	/* Allocate more space if necessary. */
	if (idx > cap) {
		seq_t *tmp = (seq_t*)malloc(idx * sizeof(*tmp));
		if (!tmp) return 0; /* allocation failure, return 0 */
		if (cache) {
			for (size_t i = 0; i < max_n; ++i)
				tmp[i] = cache[i];
			free(cache);
		}
		cache = tmp;
		cap = idx;
	}

	/* Compute new values up to n. */
	for (size_t i = max_n; i < idx; ++i) {
		seq_t digit = 3 - (seq_t)(i % 4);     /* pattern 3,2,1,0 repeating */
		if (i == 0) {
			cache[i] = digit;                /* a(1) = 3 */
		} else {
			cache[i] = cache[i - 1] * 9 + digit;
		}
	}
	max_n = idx;

	return cache[idx - 1];
}
