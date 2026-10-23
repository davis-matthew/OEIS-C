/*
 * A125372  Number of base-9 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) - 6*a(n-2) - 26*a(n-3) + 5*a(n-4) + 17*a(n-5) -
 *            a(n-6) - 3*a(n-7) (n > 7) Initial values: a(0) = 1, a(1) = 9,
 *            a(2) = 69, a(3) = 489, a(4) = 3773, a(5) = 29359, a(6) = 229371,
 *            a(7) = 1793675 For 0 ≤ n < 4, a(n) also satisfies a(n) = 4*6^n -
 *            3*5^n, but we use the explicit values abo...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms in cache */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* Convert idx_t to size_t for array indexing; safe for n <= typical limits */
    size_t idx = (size_t)n;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;             /* start with space for 8 terms */
        cache_len = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Explicit initial values up to a(7) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)69;
        cache[3] = (seq_t)489;
        cache[4] = (seq_t)3773;
        cache[5] = (seq_t)29359;
        cache[6] = (seq_t)229371;
        cache[7] = (seq_t)1793675;
    }

    /* If requested term already cached, return it */
    if (idx < cache_len)
        return cache[idx];

    /* Ensure capacity for index n */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using recurrence */
    for (size_t k = cache_len; k <= idx; ++k) {
        seq_t val = 9 * cache[k - 1]
                    - 6 * cache[k - 2]
                    - 26 * cache[k - 3]
                    + 5 * cache[k - 4]
                    + 17 * cache[k - 5]
                    - cache[k - 6]
                    - 3 * cache[k - 7];
        cache[k] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
