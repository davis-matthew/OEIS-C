/*
 * A122581  a(n) = a(n - 1) - 2*a(n - 2) + a(n - 3) - 4*a(n - 4) + 2*a(n - 5).
 * https://oeis.org/A122581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3) - 4*a(n-4) + 2*a(n-5), with
 *            a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     i8 n <= 16, i16 n <= 29, i32 n <= 55, i64 n <= 109,
 *            i128 n <= 215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;     /* pointer to allocated array */
    static idx_t max_index = 0;     /* largest index already computed */

    if (n <= 5)                         /* base values */
        return (seq_t)1;

    /* Ensure the cache is large enough for index n. */
    if (cache == NULL) {
        /* Allocate space for indices 0..n (inclusive). */
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory: return 0 (unlikely) */

        /* Initialize base terms 1..5. */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = (seq_t)1;
        max_index = 5;
    } else if (max_index < n) {
        /* Need to enlarge the cache. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out of memory: return 0 (unlikely) */
        cache = tmp;

        /* Base terms may already be set; if not, set them. */
        if (max_index < 5) {
            for (idx_t i = max_index + 1; i <= 5 && i <= n; ++i)
                cache[i] = (seq_t)1;
            if (max_index < 5)
                max_index = 5;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        cache[i] = cache[i - 1]
                 - (seq_t)2 * cache[i - 2]
                 + cache[i - 3]
                 - (seq_t)4 * cache[i - 4]
                 + (seq_t)2 * cache[i - 5];
    }
    max_index = n;

    return cache[n];
}
