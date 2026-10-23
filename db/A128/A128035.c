/*
 * A128035  a(0)=a(1)=1. a(n) = the multiple of n which is >= a(n-1)+a(n-2)
 *          and is < a(n-1)+a(n-2)+n.
 * https://oeis.org/A128035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*ceil((a(n-1)+a(n-2))/n)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = a(1) = 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache holds all terms computed so far. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache covers index n. */
    if ((size_t)n >= cache_len) {
        /* Determine new size (n+1 entries). */
        const size_t new_len = (size_t)n + 1;

        /* Reallocate cache; cast the void* result for C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {               /* allocation failed */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise base values if they were not already in place. */
        if (cache_len == 0) {            /* first call */
            cache[0] = (seq_t)1;
            if (new_len > 1)
                cache[1] = (seq_t)1;
        } else if (cache_len == 1) {     /* only a(0) known */
            cache[1] = (seq_t)1;
        }

        /* Compute all missing entries up to n. */
        const size_t start = (cache_len < 2) ? 2 : cache_len;
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            seq_t div = (seq_t)i;
            seq_t q = sum / div;
            seq_t r = sum % div;
            if (r != (seq_t)0) {
                q += (seq_t)1;
            }
            cache[i] = q * div;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
