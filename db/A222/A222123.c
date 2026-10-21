/*
 * A222123  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..4 array extended with zeros and
 *          convolved with 1,2,1.
 * https://oeis.org/A222123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + 2*a(n-5) - a(n-6) for n ≥ 7. Initial
 *            values: a(1)=2, a(2)=3, a(3)=4, a(4)=8, a(5)=12, a(6)=19
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 94, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms; 1‑based indexing. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;      /* allocated length */
    static size_t highest = 0;      /* highest index computed */

    if (n < 1)
        return (seq_t)0;            /* offset starts at 1 */

    /* Ensure the cache is large enough for index n. */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap ? cap << 1 : 8);
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        /* Copy existing values into the new cache. */
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        /* Free the old cache. */
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = highest + 1; i <= (size_t)n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)2; break;
            case 2: cache[2] = (seq_t)3; break;
            case 3: cache[3] = (seq_t)4; break;
            case 4: cache[4] = (seq_t)8; break;
            case 5: cache[5] = (seq_t)12; break;
            case 6: cache[6] = (seq_t)19; break;
            default:
                cache[i] = cache[i-1] + cache[i-3]
                           + (seq_t)2 * cache[i-5] - cache[i-6];
                break;
        }
    }
    highest = (size_t)n;

    return cache[(size_t)n];
}
