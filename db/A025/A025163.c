/*
 * A025163  The value of the associated Legendre polynomial of index n and
 *          order 1 evaluated at x=2^(-1/2) multiplied by 2^(3*n/2-1).
 * https://oeis.org/A025163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*(2*n-1)*a(n-1) - 8*n*a(n-2))/(n-1) with a(0)=0, a(1)=1
 *            (OEIS A025163)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array holding computed terms */
static idx_t  cached_n = (idx_t)-1;   /* largest index cached, (idx_t)-1 indicates empty */

/* compute a(n) using the exact recurrence, memoizing results */
seq_t a(idx_t n) {
    /* allocate or grow the cache as needed */
    if (n == (idx_t)-1) return 0;
    if (cache == NULL || cached_n < n) {
        /* new size needed */
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) exit(1); /* allocation failed */
        cache = new_cache;
        /* initialise base values if cache was empty */
        if (cached_n == (idx_t)-1) {
            cache[0] = (seq_t)0;
            if (new_len > 1) cache[1] = (seq_t)1;
        }
        /* fill new entries */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 0) { cache[0] = (seq_t)0; continue; }
            if (i == 1) { cache[1] = (seq_t)1; continue; }
            seq_t num = (seq_t)2 * ((seq_t)(2 * (seq_t)i - 1)) * cache[i - 1]
                       - (seq_t)8 * ((seq_t)i) * cache[i - 2];
            cache[i] = num / ((seq_t)(i - 1));
        }
        cached_n = n;
    }
    return cache[n];
}
