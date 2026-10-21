/*
 * A063225  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          62 ).
 * https://oeis.org/A063225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=8; for n>=3, a(n)=a(n-1)+ (n%2==0 ? 6 : 4)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* cache[1] .. cache[cache_size] are valid */
    static idx_t cache_size = 0;      /* largest index currently computed */

    /* initialize cache for the first call */
    if (cache == NULL) {
        /* reserve space for indices 1 and 2 */
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;           /* fallback to 0 if allocation fails */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache_size = 2;
    }

    /* if the requested index is already computed, return it */
    if (n <= cache_size)
        return cache[n];

    /* enlarge cache to accommodate up to n */
    if (cache_size < n) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;          /* fallback to 0 if allocation fails */
        cache = new_cache;
        /* compute new entries */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* recurrence: a(k) = a(k-1) + (k%2==0 ? 6 : 4) */
            seq_t add = (k & 1) ? (seq_t)4 : (seq_t)6;
            cache[k] = cache[k - 1] + add;
        }
        cache_size = n;
    }

    return cache[n];
}
