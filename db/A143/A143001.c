/*
 * A143001  a(0) = 0, a(1) = 1, a(n+1) = 5*(2*n+1)*a(n) + n^4*a(n-1).
 * https://oeis.org/A143001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = 5*(2*n+1)*a(n) + n^4*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the nth term of OEIS A143001.  The function caches
 * previously computed values so subsequent calls are O(1)
 * after the initial computation.  It works for any build
 * where seq_t and idx_t are defined appropriately. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;       /* array of computed terms */
    static idx_t  cache_size = 0;      /* number of terms stored (indices 0..cache_size-1) */
    static idx_t  cache_cap = 0;       /* current allocated capacity */

    /* initial setup: cache first two terms */
    if (cache == NULL) {
        cache_cap = 4; /* start with small capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory: return 0 as fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* If requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow cache if needed to at least n+1 terms */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* fallback on realloc failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms from current cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)0;
        } else if (i == 1) {
            term = (seq_t)1;
        } else {
            /* i >= 2:  a(i) = 5*(2*i-1)*a(i-1) + (i-1)^4 * a(i-2) */
            seq_t a_prev1 = cache[i-1];
            seq_t a_prev2 = cache[i-2];

            /* compute (i-1)^4 */
            seq_t s = (seq_t)(i - 1);
            s *= s;          /* (i-1)^2 */
            s *= s;          /* (i-1)^4 */

            /* term1 = 5 * (2*i-1) * a_prev1 */
            seq_t term1 = a_prev1 * (seq_t)(2 * i - 1);
            term1 *= (seq_t)5;

            /* term2 = s * a_prev2 */
            seq_t term2 = s * a_prev2;

            term = term1 + term2;
        }
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}
