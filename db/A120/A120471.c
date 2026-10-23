/*
 * A120471  a(n) = 6 * A015518(n).
 * https://oeis.org/A120471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2), a(0)=0, a(1)=6
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A120471: a(n) = 6*A015518(n) via linear recurrence */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;        /* array of seq_t values */
    static idx_t cache_cap = 0;        /* number of terms stored (size of cache) */

    /* Ensure cache is large enough for n. */
    if (cache_cap <= n)
    {
        /* grow cache to size n+1 */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;

        /* Initialize new entries */
        if (n >= 0 && cache_cap > 0)
            cache[0] = (seq_t)0;   /* a(0) = 0 */
        if (n >= 1 && cache_cap > 1)
            cache[1] = (seq_t)6;   /* a(1) = 6 */

        /* compute remaining terms up to n */
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2];
    }

    return cache[n];
}
