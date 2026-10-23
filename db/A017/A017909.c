/*
 * A017909  Expansion of 1/(1 - x^15 - x^16 - ...).
 * https://oeis.org/A017909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1..14)=0; for n>=15: a(n)=a(n-1)+a(n-15)
 * Range:     u8 n <= 63, u16 n <= 103, u32 n <= 184, u64 n <= 346,
 *            u128 n <= 670, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the recurrence derived from the generating function
   (x-1)/(x-1+x^15) and counts the number of compositions of n into
   parts ≥15. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size          */
    static size_t cache_len = 0;   /* number of values computed */

    /* Ensure we have enough space for the requested index */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t newcap = idx + 1;
        if (newcap < 16) newcap = 16;
        if (newcap < cache_cap * 2) newcap = cache_cap * 2;
        seq_t *newptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr)
            return 0;                 /* allocation failed */
        /* zero newly allocated part */
        if (newptr != cache) {
            /* If realloc moved the block, clear from old capacity */
            memset(newptr + cache_cap, 0,
                   (newcap - cache_cap) * sizeof(seq_t));
        }
        cache = newptr;
        /* If we expanded beyond previous capacity, initialise new area */
        if (cache_cap == 0) {
            memset(cache, 0, newcap * sizeof(seq_t));
        } else if (cache_cap < newcap) {
            memset(cache + cache_cap, 0,
                   (newcap - cache_cap) * sizeof(seq_t));
        }
        cache_cap = newcap;
    }

    /* Compute missing values up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i < 15) {
            cache[i] = 0;
        } else {
            cache[i] = cache[i - 1] + cache[i - 15];
        }
    }
    cache_len = idx + 1;
    return cache[idx];
}
