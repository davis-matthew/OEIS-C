/*
 * A084330  a(0)=0, a(1)=1, a(n) = 31*a(n-1) - 29*a(n-2).
 * https://oeis.org/A084330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=31*a(n-1)-29*a(n-2) (n≥2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not cached */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)0;
        } else if (i == 1) {
            val = (seq_t)1;
        } else {
            val = (seq_t)31 * cache[i - 1];
            val -= (seq_t)29 * cache[i - 2];
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
