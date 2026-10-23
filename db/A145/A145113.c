/*
 * A145113  Numbers of length n binary words with fewer than 5 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32, a(6)=64
 *            Recurrence for n≥7: a(n)=3*a(n-1)-2*a(n-2)-a(n-6)+a(n-7)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(size_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* Initialize new entries if we are extending the cache. */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0)           cache[i] = 1;
        else if (i == 1)      cache[i] = 2;
        else if (i == 2)      cache[i] = 4;
        else if (i == 3)      cache[i] = 8;
        else if (i == 4)      cache[i] = 16;
        else if (i == 5)      cache[i] = 32;
        else if (i == 6)      cache[i] = 64;
        else {
            /* recurrence: a(n)=3*a(n-1)-2*a(n-2)-a(n-6)+a(n-7) */
            seq_t v1 = cache[i-1] * 3;
            seq_t v2 = cache[i-2] * 2;
            cache[i] = v1 - v2 - cache[i-6] + cache[i-7];
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return cache[idx];
}
