/*
 * A028161  Expansion of 1/((1-4*x)*(1-9*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A028161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 34*a(n-1) - 419*a(n-2) + 2186*a(n-3) - 3960*a(n-4)  (n>=4) \
   with a(0)=1, a(1)=34, a(2)=737, a(3)=12998 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of precomputed terms */

    /* initialise the cache with the first four terms */
    if (cache_size == 0) {
        size_t init_cap = (size_t)4;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)34;
        cache[2] = (seq_t)737;
        cache[3] = (seq_t)12998;
        cache_size = 4;
    }

    /* extend the cache if necessary */
    if (n >= cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;

        /* compute values from the current size up to n */
        idx_t i = cache_size;
        if (i < 4) i = 4;                    /* first four terms already set */
        for (; i <= n; ++i) {
            seq_t term = cache[i-1] * (seq_t)34   /* + 34*a(i-1) */
                       + cache[i-3] * (seq_t)2186; /* +2186*a(i-3) */
            term -= cache[i-2] * (seq_t)419;    /* -419*a(i-2) */
            term -= cache[i-4] * (seq_t)3960;   /* -3960*a(i-4) */
            cache[i] = term;
        }
        cache_size = new_cap;
    }

    return cache[n];
}
