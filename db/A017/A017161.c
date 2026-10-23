/*
 * A017161  Expansion of g.f. 1/((1 - 3*x)*(1 - 4*x)*(1 - 12*x)).
 * https://oeis.org/A017161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 19*a(n-1) - 96*a(n-2) + 144*a(n-3), 
   a(0)=1, a(1)=19, a(2)=265 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    /* initialise with first three terms */
    if (cache == NULL) {
        cache_cap = 4;                    /* at least 3 slots, plus margin */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)265;
        cache_size = 3;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* allocation failure unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries up to index n */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)19
                     - cache[i-2] * (seq_t)96
                     + cache[i-3] * (seq_t)144;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
