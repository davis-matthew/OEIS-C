/*
 * A053524  a(n) = (6^n - (-2)^n)/8.
 * https://oeis.org/A053524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 12*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* private cache for memoization */
static seq_t *cache = NULL;          /* array holding computed values */
static idx_t cache_len = 0;          /* number of values already stored */
static idx_t cache_cap = 0;          /* capacity of the array */

/* grow the cache array to hold at least 'newlen' elements */
static void grow_cache(idx_t newlen)
{
    if (newlen <= cache_cap)
        return;
    idx_t newcap = cache_cap ? cache_cap * 2 : 4;
    while (newcap < newlen)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* ensure the cache can provide the requested index */
    if (n >= cache_len) {
        grow_cache(n + 1);

        /* seed the initial values if needed */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache_len = 1;
        }
        while (cache_len <= n) {
            if (cache_len == 1) {
                cache[1] = (seq_t)1;
                cache_len = 2;
                continue;
            }
            /* recurrence: a(k) = 4*a(k-1) + 12*a(k-2) */
            seq_t prev   = cache[cache_len - 1];
            seq_t prev2  = cache[cache_len - 2];
            cache[cache_len] = prev * (seq_t)4 + prev2 * (seq_t)12;
            ++cache_len;
        }
    }
    return cache[n];
}
