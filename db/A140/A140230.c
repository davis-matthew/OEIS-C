/*
 * A140230  Binomial transform of [1, 2, -3, -4, 5, 6, -7, -8, 9, 10, ...].
 * https://oeis.org/A140230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=2, a(4)=-6 a(n) = 4*a(n-1) - 8*a(n-2) +
 *            8*a(n-3) - 4*a(n-4) for n >= 5 This implementation memoises
 *            values in a 1‑based static array
 * Range:     i8 n <= 8, i16 n <= 23, i32 n <= 52, i64 n <= 115, i128 n <= 206,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for an out‑of‑range request (offset is 1). */
    if (n == 0)
        return 0;

    /* Static cache for memoised terms (1‑based). */
    static seq_t *cache = NULL;
    static idx_t cur_max = 0;   /* largest n already computed */
    static size_t cap = 0;      /* allocated size (>= cur_max+1) */

    /* Ensure the cache can hold at least the first 4 terms. */
    if (cache == NULL) {
        cap = 5; /* indices 0..4 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        /* Initialise the first four sequence values. */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)-6;
        cur_max = 4;
    }
    /* Grow the cache if needed. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Return cached values that are already known. */
    if (n <= cur_max)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1]
                 - 8 * cache[i - 2]
                 + 8 * cache[i - 3]
                 - 4 * cache[i - 4];
    }
    cur_max = n;

    return cache[n];
}
