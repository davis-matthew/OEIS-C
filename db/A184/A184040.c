/*
 * A184040  1/9 the number of (n+1) X (n+1) 0..2 arrays with all 2 X 2
 *          subblocks having the same four values.
 * https://oeis.org/A184040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4) with a(1)=9, a(2)=21,
 *            a(3)=41, a(4)=81
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;              /* memoised terms */
    static size_t cache_cap = 0;             /* allocated capacity */
    static idx_t  cache_len = 0;             /* highest index computed */

    /* Ensure space up to index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (cache_cap == 0) new_cap = 16;
        else if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)9;
        else if (i == 2)
            cache[i] = (seq_t)21;
        else if (i == 3)
            cache[i] = (seq_t)41;
        else if (i == 4)
            cache[i] = (seq_t)81;
        else
            cache[i] = (seq_t)3 * cache[i-1]
                     - (seq_t)6 * cache[i-3]
                     + (seq_t)4 * cache[i-4];
    }

    cache_len = n;
    return cache[n];
}
