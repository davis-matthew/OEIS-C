/*
 * A223396  4 X 4 square grid graph coloring a rectangular array: number of n
 *          X 2 0..15 arrays where 0..15 label nodes of the square grid graph
 *          and every array movement to a horizontal or vertical neighbor
 *          moves along an edge of this graph.
 * https://oeis.org/A223396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 18*a(n-2) - 9*a(n-3) + 2*a(n-4); Initial
 *            values: a(1)=48, a(2)=328, a(3)=2360, a(4)=17280
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Static cache for computed values. */
    static seq_t *cache      = NULL;   /* dynamic array of seq_t values */
    static idx_t  cache_size = 0;      /* allocated size of the array   */
    static idx_t  cache_known = 0;     /* highest index that has been computed */

    /* First call: allocate and initialise the base values. */
    if (cache == NULL) {
        idx_t init_size = n > 4 ? n + 1 : 5;      /* at least indices 0..4 */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0;                    /* allocation failure */

        cache_size = init_size;
        cache[1] = (seq_t)48;     /* a(1) */
        cache[2] = (seq_t)328;    /* a(2) */
        cache[3] = (seq_t)2360;   /* a(3) */
        cache[4] = (seq_t)17280;  /* a(4) */
        cache_known = 4;
    }

    /* Grow the cache if necessary to accommodate the requested index. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Return the value if it has already been computed. */
    if (n <= cache_known) return cache[n];

    /* Compute missing values using the recurrence. */
    for (idx_t i = cache_known + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)10 * cache[i-1]
            - (seq_t)18 * cache[i-2]
            - (seq_t)9  * cache[i-3]
            + (seq_t)2  * cache[i-4];
    }
    cache_known = n;
    return cache[n];
}
