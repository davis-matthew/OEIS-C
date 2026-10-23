/*
 * A154996  a(n) = 5*a(n-1) + 20*a(n-2), n>2 ; a(0)=1, a(1)=1, a(2)=9.
 * https://oeis.org/A154996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 20*a(n-2) with a(0)=1, a(1)=1, a(2)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache keeps all computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of allocated elements */
    static idx_t  last_computed = 0;/* highest index already computed */

    /* Ensure the cache is large enough for the requested index.
       We always need at least space for indices 0,1,2.          */
    idx_t needed = (n >= 2) ? (n + 1) : 3;    /* include indices 0-2 */
    if (needed > cache_size) {
        idx_t new_size = (cache_size == 0) ? 3 : cache_size;
        while (new_size < needed)
            new_size <<= 1;          /* grow exponentially */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                    /* fallback to malloc on failure   */
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values when first needed.  The cache is
       guaranteed to have space for indices 0, 1 and 2.           */
    if (last_computed < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        last_computed = 2;
    }

    /* Compute missing terms up to n, if any. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)20 * cache[i - 2];
    }
    last_computed = n;

    return cache[n];
}
