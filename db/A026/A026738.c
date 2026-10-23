/*
 * A026738  Expansion of 1/((1-2*x)*(1-6*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A026738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Expansion of 1/((1-2*x)*(1-6*x)*(1-8*x)*(1-12*x))
   Formula: a(n) = (18*12^(n+1) - 5*8^(n+2) + 135*6^n - 2^n)/30
   Recurrence: a(0)=1, a(1)=28, a(2)=516, a(3)=7952
               a(n)=28*a(n-1)-268*a(n-2)+1008*a(n-3)-1152*a(n-4)  (n≥4) */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of values already computed */

    /* Grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Initialise cache to zero for safety */
    if (cache_size == 0) {
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
    }

    /* Compute terms up to n using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1)       cache[i] = (seq_t)28;
        else if (i == 2)       cache[i] = (seq_t)516;
        else if (i == 3)       cache[i] = (seq_t)7952;
        else
            cache[i] = cache[i-1]*28 - cache[i-2]*268 + cache[i-3]*1008 - cache[i-4]*1152;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
