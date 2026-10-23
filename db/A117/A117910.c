/*
 * A117910  Expansion of (1 + x + x^2 + x^4)/((1-x^3)*(1-x^6)).
 * https://oeis.org/A117910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + a(n-6) - a(n-9) (for n >= 9) Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=1, a(6)=2, a(7)=3,
 *            a(8)=2
 * Range:     u8 n <= 765, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;          /* pointer to cached values */
    static idx_t  cache_len = 0;         /* number of values currently in cache */
    static idx_t  cache_cap = 0;         /* allocated capacity of cache */

    /* Return cached value if already stored */
    if (n < cache_len)
        return cache[n];

    /* Ensure capacity at least n+1 */
    idx_t need = n + 1;
    if (need > cache_cap) {
        /* Grow capacity (doubling strategy) */
        idx_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
        if (new_cap < need)
            new_cap = need;
        /* Reallocate cache */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort (building blocks usually have abort behavior) */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initial sequence values for indices 0..8 */
    const seq_t init[9] = {1, 1, 1, 1, 2, 1, 2, 3, 2};

    /* Fill in missing values up to cache_len (may be > n+1) */
    for (idx_t i = cache_len; i < cache_cap; ++i) {
        if (i < 9) {
            cache[i] = init[i];
        } else {
            /* Recurrence: a(i) = a(i-3) + a(i-6) - a(i-9) */
            cache[i] = cache[i - 3] + cache[i - 6] - cache[i - 9];
        }
    }

    /* Update cache length to at least n+1 */
    if (cache_len < need) {
        /* Use the computed values for indices cache_len .. need-1 */
        for (idx_t i = cache_len; i < need; ++i) {
            /* They were already computed in the loop above */
            /* No action needed here */
        }
        cache_len = need;
    }

    return cache[n];
}
