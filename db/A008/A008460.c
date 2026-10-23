/*
 * A008460  Take sum of squares of digits of previous term; start with 6.
 * https://oeis.org/A008460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6; a(n)=sum of squares of digits of a(n-1) */
#include "../utils/grow_seq.h"
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to zero-based position */
    idx_t pos = n - 1;

    /* Static cache to allow arbitrary order calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of values stored */

    /* Ensure cache can hold required length */
    if (pos >= (idx_t)cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)(pos + 1));

    /* Initialize first element if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)6;
        cache_len = 1;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= pos; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t sum  = 0;
        seq_t tmp  = prev;

        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % (seq_t)10);
            sum += (seq_t)d * (seq_t)d;
            tmp /= (seq_t)10;
        }

        /* Handle the special case prev == 0 → sum stays 0 */
        cache[(size_t)i] = sum;
    }

    cache_len = pos + 1;

    return cache[(size_t)pos];
}
