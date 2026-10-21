/*
 * A233726  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 6 and no adjacent elements equal.
 * https://oeis.org/A233726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 5*a(n-2) - 7*a(n-3) - 2*a(n-4)
 * Initial values: a(1)=40, a(2)=148, a(3)=556, a(4)=2104
 * (The recurrence holds for n ≥ 5)
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[1]..cache[computed] contain values already
     * computed.  cache[0] is unused.  The cache is grown on demand and
     * retained between calls so that repeated calls run in O(1) time
     * after the first lookup. */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t cache_len = 0;   /* allocated size (indexable up to cache_len-1) */
    static idx_t computed = 0;    /* largest index already computed */

    if (n < 1)   /* undefined for n<1 */
        return (seq_t)0;

    /* Initialise cache on first call.  The cache is allocated at least
     * up to index 4 to store the initial four terms. */
    if (cache == NULL) {
        cache_len = 5;                      /* indices 0..4 inclusive */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        cache[1] = (seq_t)40;
        cache[2] = (seq_t)148;
        cache[3] = (seq_t)556;
        cache[4] = (seq_t)2104;
        computed = 4;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if (n + 1 > cache_len) {
        idx_t new_len = (n + 1) * 2;     /* double to amortise future reallocs */
        if (new_len < 5) new_len = 5;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms using the empirical recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)3 * cache[i - 1] + (seq_t)5 * cache[i - 2];
        seq_t t2 = (seq_t)7 * cache[i - 3] + (seq_t)2 * cache[i - 4];
        cache[i] = t1 - t2;
    }
    computed = n > computed ? n : computed;

    return cache[n];
}
