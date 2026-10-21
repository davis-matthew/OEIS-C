/*
 * A077398  First member of the Diophantine pair (m,k) that satisfies
 *          7*(m^2+m) = k^2+k; a(n)=m.
 * https://oeis.org/A077398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-2) - a(n-4) + 7 for n >= 4 Initial terms: a(0)=0,
 *            a(1)=2, a(2)=5, a(3)=39 (These are the first four members of the
 *            OEIS sequence A077398.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache is large enough to hold indices 0..n. */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        if (new_len < 4) new_len = 4;          /* need space for first 4 terms */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        /* Initialise any new entries that have not yet been set. */
        if (cache_len == 0) {      /* first time */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)5;
            cache[3] = (seq_t)39;
        } else {
            /* If the cache was small, fill missing first terms. */
            if (cache_len < 4) {
                if (cache_len <= 0) cache[0] = (seq_t)0;
                if (cache_len <= 1) cache[1] = (seq_t)2;
                if (cache_len <= 2) cache[2] = (seq_t)5;
                if (cache_len <= 3) cache[3] = (seq_t)39;
            }
        }
        /* Compute new terms up to n using the recurrence. */
        size_t start = cache_len;
        if (start < 4) start = 4;   /* recurrence only valid from index 4 */
        for (size_t i = start; i <= n; ++i) {
            /* The expression fits in seq_t because the sequence is monotonically
               increasing and the subtraction is always non‑negative for n >= 4. */
            cache[i] = (seq_t)16 * cache[i - 2] - cache[i - 4] + (seq_t)7;
        }
        cache_len = new_len;
    }
    return cache[n];
}
