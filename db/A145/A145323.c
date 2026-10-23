/*
 * A145323  Numbers x such that there exists n in N : (x+1)^3-x^3=31*n^2.
 * https://oeis.org/A145323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=22, a(2)=546817; a(n)=24302*a(n-1)-a(n-2)+12150 for n>2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n≥1 */

    /* static cache of computed terms, 0‑based index (a(1) at cache[0]) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of terms stored */

    size_t needed = (size_t)n;             /* n is bounded by SIZE_MAX */

    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache = new_cache;

        /* initialise base values if the cache was empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)22;
            if (needed > 1) cache[1] = (seq_t)546817;
            for (size_t i = 2; i < needed; ++i) {
                cache[i] = (seq_t)24302 * cache[i-1] - cache[i-2] + (seq_t)12150;
            }
        } else {
            /* if we are just adding the second term */
            if (cache_size == 1 && needed > 1) {
                cache[1] = (seq_t)546817;
            }
            for (size_t i = cache_size; i < needed; ++i) {
                /* skip i==1 because a(2) is already set */
                if (i < 2) continue;
                cache[i] = (seq_t)24302 * cache[i-1] - cache[i-2] + (seq_t)12150;
            }
        }
        cache_size = needed;
    }
    return cache[n-1];                     /* a(n) stored at index n-1 */
}
