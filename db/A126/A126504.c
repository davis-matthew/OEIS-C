/*
 * A126504  Number of base 9 n-digit numbers with adjacent digits differing by
 *          four or less.
 * https://oeis.org/A126504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) + a(n-2) - 6*a(n-3) + a(n-5) for n>5, a(0)=1,
 *            a(1)=9, a(2)=61, a(3)=429, a(4)=3011, a(5)=21141
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;         /* current allocated size      */
    static idx_t  cache_last = (idx_t)-1;/* largest index stored       */

    /* Ensure cache is large enough to hold base values */
    if (cache == NULL) {
        /* Allocate space for at least the first 6 terms (0..5) */
        cache_cap = 6U;  /* n=5 -> 6 entries */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* fatal, but return 0 on allocation failure */

        /* Initialize known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)61;
        cache[3] = (seq_t)429;
        cache[4] = (seq_t)3011;
        cache[5] = (seq_t)21141;
        cache_last = 5;
    }

    /* Grow cache if needed for the requested index */
    if (n > cache_last) {
        if ((size_t)n >= cache_cap) {
            size_t new_cap = (size_t)n + 1U;
            size_t old_cap = cache_cap;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
            /* Zero new entries to be safe */
            for (size_t i = old_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        }
        /* Compute missing values using the recurrence */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* i >= 6 here, because cache_last >=5 */
            seq_t val = (7 * cache[i-1]) + cache[i-2]
                        - (6 * cache[i-3]) + cache[i-5];
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[(size_t)n];
}
