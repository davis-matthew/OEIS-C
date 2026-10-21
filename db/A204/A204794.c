/*
 * A204794  Number of (n+1) X 3 0..1 arrays with rows and columns of
 *          determinants of all 2 X 2 subblocks lexicographically
 *          nondecreasing.
 * https://oeis.org/A204794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 5*a(n-2) - 12*a(n-3) + 12*a(n-4)  (n > 8)  
   Initial values: a(1)=47, a(2)=240, a(3)=1001, a(4)=4273,
   a(5)=18550, a(6)=81009, a(7)=353975, a(8)=1547377. */

#include <stdlib.h>

/* Compute the sequence value for index n (1‑based). */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Pre‑defined first eight terms. */
    static const seq_t init_vals[8] = {
        (seq_t)47, (seq_t)240, (seq_t)1001, (seq_t)4273,
        (seq_t)18550, (seq_t)81009, (seq_t)353975, (seq_t)1547377
    };

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = ((size_t)n > 1 ? (size_t)n * 2 : 2);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to index n. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = 6 * cache[i-1] - 5 * cache[i-2]
                     - 12 * cache[i-3] + 12 * cache[i-4];
        }
    }
    cache_len = (size_t)n;

    /* Return the requested term (a(1) is at cache[0]). */
    return cache[(size_t)n - 1];
}
