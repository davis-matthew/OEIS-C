/*
 * A075844  Numbers k such that 11*k^2 + 4 is a square.
 * https://oeis.org/A075844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=6; a(n)=20*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use a static cache so subsequent calls are fast. */
    static seq_t *cache   = NULL;   /* array of computed values */
    static size_t cache_len = 0;    /* number of elements stored (highest index + 1) */
    static size_t cache_cap = 0;    /* allocated capacity */

    size_t idx = (size_t)n;          /* convert index to size_t */

    /* Extend cache if needed. */
    if (idx >= cache_len) {
        /* Ensure capacity at least idx+1. */
        if (cache_cap < idx + 1) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while (new_cap <= idx) new_cap <<= 1;      /* grow exponentially */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);                         /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise missing base values. */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache_len = 1;
        }
        if (cache_len == 1 && idx >= 1) {
            cache[1] = (seq_t)6;
            cache_len = 2;
        }

        /* Compute any additional terms up to idx. */
        for (size_t i = cache_len; i <= idx; ++i) {
            cache[i] = cache[i - 1] * (seq_t)20 - cache[i - 2];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
