/*
 * A017879  Expansion of 1/(1-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9)+a(n-10)+a(n-11)+a(n-12) with a(0)=1, a(1..8)=0,
 *            a(9)=1, a(10)=1, a(11)=1, a(12)=1 for n≥13
 * Range:     u8 n <= 59, u16 n <= 100, u32 n <= 184, u64 n <= 350,
 *            u128 n <= 684, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n < 13 */
    static const seq_t init[13] = {1,0,0,0,0,0,0,0,0,1,1,1,1};
    if (n < 13) return init[(size_t)n];

    /* Caching for repeated queries */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;   /* largest index stored in cache */

    size_t idx = (size_t)n;   /* n is guaranteed to fit in size_t here */

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache_cap = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure */
    }

    /* Ensure the first 13 values are present when needed */
    if (cache_max < 12) {
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        cache_max = 12;
    }

    /* Grow cache if the requested index exceeds current capacity */
    if (idx > cache_max) {
        if (idx >= cache_cap) {
            /* Double the capacity until it fits idx */
            while (idx >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0) new_cap = 1;  /* guard against 0 */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) break;              /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
        }

        /* Compute missing values up to idx */
        for (size_t k = cache_max + 1; k <= idx; ++k) {
            cache[k] = cache[k-9] + cache[k-10] + cache[k-11] + cache[k-12];
        }
        cache_max = idx;
    }

    return cache[idx];
}
