/*
 * A118406  Unsigned row sums of triangle A118404.
 * https://oeis.org/A118406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118406 – Unsigned row sums of triangle A118404.
   Recurrence (for n ≥ 5): a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5)
   Initial values: a(0)=1, a(1)=2, a(2)=2, a(3)=4, a(4)=4
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;       /* array of previous terms   */
    static idx_t cache_size = 0;      /* number of terms stored    */
    static size_t cache_cap = 0;      /* allocated capacity        */

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache is large enough to hold term n */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap *= 2;

        /* Use standard realloc; cast to seq_t* for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure; return 0 */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms from cache_size up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        switch (i) {
            case 0: cache[0] = 1; break;
            case 1: cache[1] = 2; break;
            case 2: cache[2] = 2; break;
            case 3: cache[3] = 4; break;
            case 4: cache[4] = 4; break;
            default: {
                /* a(i) = 2*a(i-1) + a(i-4) - 2*a(i-5) */
                seq_t val = cache[i - 1];
                val += val;                 /* 2*a(i-1)   */
                val += cache[i - 4];         /* + a(i-4)   */
                seq_t sub = cache[i - 5];
                sub += sub;                 /* 2*a(i-5)   */
                val -= sub;                 /* subtract   */
                cache[i] = val;
            }
        }
    }

    cache_size = (idx_t)(n + 1);
    return cache[n];
}
