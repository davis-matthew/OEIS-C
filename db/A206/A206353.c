/*
 * A206353  Number of n X 2 0..4 arrays with no element equal to another
 *          within two positions in the same row or column, and new values
 *          0..4 introduced in row major order.
 * https://oeis.org/A206353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 13*a(n-2) - 18*a(n-3) (for n > 5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(1) = 1, a(2) = 4, a(3) = 17, a(4) = 120, a(5) = 884 */

#include <stdlib.h>

/* Cached values:  cache[i] holds a(i+1)  (0‑based index) */
static seq_t *cache = NULL;
static idx_t  cached_n = 0;

/* Ensure the cache holds values up to at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n)
        return;

    /* first time: allocate initial cache for 5 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL)
            return;           /* allocation failure – caller will see 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)120;
        cache[4] = (seq_t)884;
        cached_n = 5;
    }

    /* extend cache if necessary */
    if (n > cached_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (new_cache == NULL)
            return;           /* allocation failure – cache unchanged */
        cache = new_cache;

        /* compute new terms using the recurrence */
        for (idx_t i = cached_n; i < n; ++i) { /* i is 0‑based index */
            /* a(i+1) = 6*a(i) + 13*a(i-1) - 18*a(i-2)  for i >= 5 */
            seq_t t = cache[i - 1];
            t *= (seq_t)6;
            t += cache[i - 2] * (seq_t)13;
            t -= cache[i - 3] * (seq_t)18;
            cache[i] = t;
        }
        cached_n = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1 – undefined for n=0 */

    if (n <= 5) {
        /* return hard‑coded initial values */
        switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)17;
        case 4: return (seq_t)120;
        case 5: return (seq_t)884;
        }
    }

    /* ensure cache has the value for n */
    ensure_cache(n);
    if (n > cached_n)          /* allocation failure */
        return 0;
    return cache[n - 1];
}
