/*
 * A065124  a(n) = (sum of digits of a(n-2)) + a(n-1); a(0) = 0 and a(1) = 1.
 * https://oeis.org/A065124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=digit_sum(a(n-2))+a(n-1); */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of initialized entries */

    size_t target = (size_t)n;

    /* Grow the cache if necessary */
    if (target >= cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp)            /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure the first two elements are initialized */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    if (cache_len == 1 && target >= 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute any missing values up to the requested index */
    while (cache_len <= target) {
        seq_t val = digit_sum(cache[cache_len - 2]) + cache[cache_len - 1];
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[target];
}
