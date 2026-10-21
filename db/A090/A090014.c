/*
 * A090014  Permanent of (0,1)-matrix of size n X (n+d) with d=4 and n-1 zeros
 *          not on a line.
 * https://oeis.org/A090014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (n+3)*a(n-1) + (n-2)*a(n-2), a(1)=5, a(2)=25 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* guard against the (unlikely) use of a(0) */
    if (n == 0) return 0;

    /* static cache for memoisation of already computed terms */
    static seq_t *cache      = NULL;  /* array of a(1), a(2), … */
    static idx_t cache_len   = 0;     /* number of terms stored (>=1) */

    /* initialise the cache for the first two terms */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed */
        cache[0] = (seq_t)5;                /* a(1) */
        cache[1] = (seq_t)25;               /* a(2) */
        cache_len = 2;
    }

    /* if the requested term is already memoised, return it */
    if (n <= cache_len) return cache[n - 1];

    /* enlarge the cache to hold up to term n */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache = NULL;
        cache_len = 0;
        return 0;                            /* allocation failed */
    }
    cache = tmp;

    /* compute the missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i corresponds to the index (0‑based) of a(i+1) */
        idx_t idx = i + 1;                   /* n‑value of this term */

        /* a(idx) = (idx+3)*a(idx-1) + (idx-2)*a(idx-2) */
        seq_t term1 = (seq_t)(idx + 3) * cache[idx - 2];
        seq_t term2 = (seq_t)(idx - 2) * cache[idx - 3];
        cache[i] = term1 + term2;
    }

    cache_len = n;
    return cache[n - 1];
}
