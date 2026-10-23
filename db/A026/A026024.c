/*
 * A026024  Expansion of 1/((1-2*x)*(1-5*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A026024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026024: Expansion of 1/((1-2*x)*(1-5*x)*(1-9*x)*(1-10*x))
   Recurrence: a(0)=1, a(1)=26, a(2)=443, a(3)=6280
   a(n) = 26*a(n-1) - 233*a(n-2) + 820*a(n-3) - 900*a(n-4)  (n≥4) */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;   /* number of values stored */
    static size_t capacity = 0;     /* allocated array size (in seq_t elements) */

    /* initial base values */
    if (cached_len == 0) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)443;
        cache[3] = (seq_t)6280;
        cached_len = 4;
    }

    /* if requested value already cached, return it */
    if (n < cached_len) return cache[n];

    /* grow cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache     = tmp;
        capacity  = new_cap;
    }

    /* compute terms up to n (inclusive) */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] = (seq_t)26   * cache[i-1]
                 - (seq_t)233  * cache[i-2]
                 + (seq_t)820  * cache[i-3]
                 - (seq_t)900  * cache[i-4];
    }
    cached_len = n + 1;
    return cache[n];
}
