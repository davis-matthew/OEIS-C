/*
 * A122589  Expansion of 1/(1 - 11*x + 45*x^2 - 84*x^3 + 70*x^4 - 21*x^5 +
 *          x^6).
 * https://oeis.org/A122589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 11*a(n-1) -
 *            45*a(n-2) + 84*a(n-3) - 70*a(n-4) + 21*a(n-5) - a(n-6) * with
 *            initial values a(0)=1, a(1)=11, a(2)=76, a(3)=425, a(4)=2109,
 *            a(5)=9709. * This is the expansion of 1/(1 - 11*x + 45*x^2 -
 *            84*x^3 + 70*x^4 - 21*x^5 + x^6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_used = 0;   /* number of values initialised */
static idx_t cache_cap  = 0;   /* allocated size of cache */

/* grow the cache to at least n+1 elements */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* initialise first six terms */
static void init_cache(void)
{
    ensure_cache_size(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)11;
    cache[2] = (seq_t)76;
    cache[3] = (seq_t)425;
    cache[4] = (seq_t)2109;
    cache[5] = (seq_t)9709;
    cache_used = 6;
}

seq_t a(idx_t n)
{
    if (!cache) init_cache();
    if (n < cache_used) return cache[n];

    ensure_cache_size(n);
    for (idx_t i = cache_used; i <= n; ++i)
    {
        /* Compute using the recurrence:
         * a(i) = 11*a(i-1) - 45*a(i-2) + 84*a(i-3) - 70*a(i-4) + 21*a(i-5) - a(i-6) */
        seq_t term = mul_small(cache[i-1], 11)
                   - mul_small(cache[i-2], 45)
                   + mul_small(cache[i-3], 84)
                   - mul_small(cache[i-4], 70)
                   + mul_small(cache[i-5], 21)
                   - cache[i-6];
        cache[i] = term;
    }
    cache_used = n + 1;
    return cache[n];
}
