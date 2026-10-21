/*
 * A220221  Odd positive integers k such that k^2 has at most three nonzero
 *          binary digits.
 * https://oeis.org/A220221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 2*a(n-2)  for n > 9
 * Initial terms (n = 1 … 9) are:
 *   1, 3, 5, 7, 9, 17, 23, 33, 65
 * This implementation memoises the sequence so that calls with
 * arbitrary indices, even out of order, are fast.
 */
#include <stdlib.h>

/* Static cache of computed terms, 1‑based (cache[0] holds a(1)). */
static seq_t *seq_cache = NULL;   /* pointer to the cache array */
static idx_t  cache_len = 0;      /* number of terms currently cached */

/* Base terms for n <= 9. */
static const seq_t base_terms[9] = {
    (seq_t)1,  (seq_t)3,  (seq_t)5,  (seq_t)7,
    (seq_t)9,  (seq_t)17, (seq_t)23, (seq_t)33,
    (seq_t)65
};

seq_t a(idx_t n)
{
    /* n must be at least 1; for safety we clamp it to 1. */
    if (n < 1)
        return (seq_t)0;

    /* Expand the cache if the requested index is not yet cached. */
    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_cache)
            return (seq_t)0;   /* Allocation failure – unlikely. */

        /* Copy already cached terms. */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];

        /* Initialise the base terms that are not yet cached. */
        for (idx_t i = cache_len; i < 9 && i < new_len; ++i)
            new_cache[i] = base_terms[i];

        /* Compute remaining terms using the recurrence. */
        idx_t start = cache_len > 9 ? cache_len : 9;
        for (idx_t i = start; i < new_len; ++i) {
            seq_t prev1 = new_cache[i - 1];   /* a(i)   */
            seq_t prev2 = new_cache[i - 2];   /* a(i-1) */
            new_cache[i] = prev1 * (seq_t)3 - prev2 * (seq_t)2;
        }

        /* Replace the old cache. */
        free(seq_cache);
        seq_cache = new_cache;
        cache_len = new_len;
    }

    return seq_cache[n - 1];
}
