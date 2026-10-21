/*
 * A220726  Number of ways to reciprocally link elements of a 2 X n array
 *          either to themselves or to exactly two horizontal, diagonal or
 *          antidiagonal neighbors.
 * https://oeis.org/A220726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-4); Initial values: a(1)=1,
 *            a(2)=2, a(3)=7, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* 1‑based cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements)   */
    static idx_t  cache_max = 0;   /* highest index computed so far      */

    if (n == 0) return 0;          /* offset below 1 is irrelevant      */

    /* Allocate or expand the cache as needed */
    if (cache == NULL) {
        cache_cap = (n + 1 > 5) ? (n + 1) : 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);       /* allocation failure – abort      */
    } else if ((size_t)(n + 1) > cache_cap) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(1);               /* allocation failure – abort      */
        }
        cache = tmp;
    }

    /* Initialise known terms once */
    if (cache_max < 4) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)21;
        cache_max = 4;
    }

    /* Return if the requested term is already cached */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)((seq_t)3 * cache[i - 1]
                           + cache[i - 2]
                           - (seq_t)2 * cache[i - 4]);
    }
    cache_max = n;
    return cache[n];
}
