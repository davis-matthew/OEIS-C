/*
 * A156160  a(n) = 34*a(n-1)-a(n-2)-2312 for n > 2; a(1)=169, a(2)=2809.
 * https://oeis.org/A156160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-1)-a(n-2)-2312; a(1)=169, a(2)=2809
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache to support arbitrary ordering of calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n computed so far */

    if (n < 1)
        return 0; /* undefined for n<1 – behavior below the offset is irrelevant */

    /* initialise cache on first use */
    if (cache == NULL) {
        /* start with capacity for indices 0..2 (we ignore index 0) */
        size_t init_cap = 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure; caller will get 0 */
        cache_size = 2;
        cache[1] = (seq_t)169;
        cache[2] = (seq_t)2809;
    }

    /* return already computed value */
    if (n <= cache_size)
        return cache[n];

    /* grow the cache up to n */
    size_t old_cap = cache_size + 1;          /* indices 0..cache_size */
    size_t new_cap = old_cap * 2;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    new_cap += 1; /* extra slot for index 0 */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)34;
        term -= cache[i - 2];
        term -= (seq_t)2312;
        cache[i] = term;
    }

    cache_size = n;
    return cache[n];
}
