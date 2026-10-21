/*
 * A218838  Unmatched value maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..1 n X 4 array.
 * https://oeis.org/A218838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 5*a(n-2) + 24*a(n-3) - 9*a(n-4) + 9*a(n-5) for
 *            n > 6 Initial values: a(1) = 7 a(2) = 57 a(3) = 453 a(4) = 3617
 *            a(5) = 28888 a(6) = 230726
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* cached values, 1‑based index */
    static idx_t cached_max = 0;  /* highest index computed so far */
    static size_t cache_size = 0; /* allocated size of cache (>= cached_max+1) */

    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n=0, but return 0 for safety */

    /* initialise cache with first six terms if not yet done */
    if (cached_max == (idx_t)0) {
        cache_size = 7 + 1;                     /* indices 0..7 used, 0 unused */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */

        cache[1] = (seq_t)7;
        cache[2] = (seq_t)57;
        cache[3] = (seq_t)453;
        cache[4] = (seq_t)3617;
        cache[5] = (seq_t)28888;
        cache[6] = (seq_t)230726;
        cached_max = 6;
    }

    /* enlarge the cache if necessary */
    if (cache_size <= (size_t)n + 1) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n + 1)
            new_size = new_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        /* zero initialise the new part */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    while (cached_max < n) {
        /* compute a(cached_max+1) */
        seq_t next = (seq_t)7 * cache[cached_max]          /* a(n-1) */
                    + (seq_t)5 * cache[cached_max - 1]      /* a(n-2) */
                    + (seq_t)24 * cache[cached_max - 2]     /* a(n-3) */
                    - (seq_t)9 * cache[cached_max - 3]     /* a(n-4) */
                    + (seq_t)9 * cache[cached_max - 4];    /* a(n-5) */
        ++cached_max;
        cache[cached_max] = next;
    }

    return cache[n];
}
