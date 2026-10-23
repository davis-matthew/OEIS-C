/*
 * A161583  The list of the k values in the common solutions to the 2
 *          equations 15*k+1 = A^2, 19*k+1 = B^2.
 * https://oeis.org/A161583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 288*(a(n+2)-a(n+1)) + a(n); Initial values: a(1)=0,
 *            a(2)=17, a(3)=4896
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms, 1‑based indexing */
    static idx_t cache_cap   = 0;    /* allocated size of cache        */
    static idx_t cache_last  = 0;    /* highest n with computed value  */

    /* ensure enough space and initialise first terms */
    if (cache_last == 0) {
        /* minimal capacity to hold indices 1..3 */
        cache_cap = (n + 1 > 4) ? (n + 1) : 4U;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)4896;
        cache_last = 3;
    }

    /* grow cache if requested n is beyond current capacity */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* memory allocation failed – aborting is safe for this context */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = (seq_t)288 * (cache[i-1] - cache[i-2]) + cache[i-3];
        }
        cache_last = n;
    }

    return cache[n];
}
