/*
 * A183775  Half the number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          having exactly 2 ones.
 * https://oeis.org/A183775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=13, a(2)=47; for n>=3 a(n) = 3*a(n-1) + 4*a(n-2) -
 *            8*a(n-3) (OEIS A183775)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated slots              */
    static size_t cache_used = 0; /* first slot beyond the last
                                    * value that has been written   */

    /* +1 because we store a(n) in cache[n] */
    size_t idx = (size_t)n;
    /* ensure we can write up to n (and maybe up to 2 for the bases)   */
    size_t max_needed = idx;
    if (cache_used == 0 && max_needed < 3)
        max_needed = 2;               /* need slots 0..2 for a(0..2) */

    if (max_needed >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= max_needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first three terms once */
    if (cache_used == 0) {
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)47;
        cache_used = 3;
    }

    /* compute any missing terms up to n */
    if (cache_used <= idx) {
        for (size_t i = cache_used; i <= idx; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1]
                     + (seq_t)4 * cache[i - 2]
                     - (seq_t)8 * cache[i - 3];
        }
        cache_used = idx + 1;
    }

    return cache[idx];
}
