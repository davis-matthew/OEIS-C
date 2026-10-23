/*
 * A014937  a(1)=1, a(n) = n*20^(n-1) + a(n-1).
 * https://oeis.org/A014937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 41, a(n) = 40*a(n-1) - 400*a(n-2) + 1 for n > 2
 *            (equivalent to the closed form a(n) = (20^n*(19*n-1)+1)/361)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 – return 0 */

    static seq_t *cache  = NULL;  /* 1‑based array  */
    static size_t cache_size = 0; /* number of allocated slots */

    /* Expand cache if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – fail quietly */
        cache = tmp;

        /* Initialise newly allocated entries */
        if (cache_size == 0) {
            cache[0] = 0;          /* unused */
            cache[1] = 1;
            if (n >= 2) cache[2] = 41;
            idx_t start = (n < 3 ? n + 1 : 3);
            for (idx_t i = start; i <= n; ++i)
                cache[i] = 40 * cache[i-1] - 400 * cache[i-2] + 1;
        } else {
            /* cache already holds values up to cache_size-1 */
            idx_t start = cache_size;
            if (start <= 1) { cache[1] = 1; start = 2; }
            if (start <= 2) { cache[2] = 41; start = 3; }
            for (idx_t i = start; i <= n; ++i)
                cache[i] = 40 * cache[i-1] - 400 * cache[i-2] + 1;
        }

        cache_size = new_size;
    }

    return cache[n];
}
