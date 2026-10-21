/*
 * A061184  Fifth (m=4) column of triangle A060921 (bisection of Fibonacci
 *          triangle, odd part).
 * https://oeis.org/A061184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A061184: Fifth (m=4) column of triangle A060921
    Bisection of Fibonacci triangle, odd part.
    g.f.: (5*(1+x^4)-10*(x+x^3)+11*x^2)/(1-3*x+x^2)^5
    Recurrence (derived from the denominator):
        a(n) = 15 a(n-1) -95 a(n-2) +330 a(n-3) -685 a(n-4)
               +873 a(n-5) -685 a(n-6) +330 a(n-7) -95 a(n-8)
               +15 a(n-9) - a(n-10)   for n ≥ 10
    Initial values (n = 0 … 9):
        5, 65, 511, 3130, 16435, 77645, 339535, 1399478, 5504650, 20845300
*/

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache for computed terms */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of terms currently stored   */
static idx_t  cache_cap = 0;   /* allocated capacity (elements)      */

/* Ensure the cache can hold at least `needed` elements */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 10;
    if (new_cap < needed)
        new_cap = needed;
    /* C++ requires an explicit cast from void* */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();                   /* allocation failure */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Initialise the cache with the first ten terms of the sequence */
static void init_cache(void)
{
    if (cache_len > 0)
        return;                     /* already initialised */
    ensure_capacity(10);
    cache[0] = (seq_t)5ULL;
    cache[1] = (seq_t)65ULL;
    cache[2] = (seq_t)511ULL;
    cache[3] = (seq_t)3130ULL;
    cache[4] = (seq_t)16435ULL;
    cache[5] = (seq_t)77645ULL;
    cache[6] = (seq_t)339535ULL;
    cache[7] = (seq_t)1399478ULL;
    cache[8] = (seq_t)5504650ULL;
    cache[9] = (seq_t)20845300ULL;
    cache_len = 10;
}

/* Function requested by the build */
seq_t a(idx_t n)
{
    if (n >= cache_cap)
        ensure_capacity(n + 1);
    init_cache();

    if (n < cache_len)
        return cache[n];

    /* Compute terms up to n sequentially */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 10) { /* safety check – shouldn't happen */
            /* nothing to do, value already in cache */
        } else {
            seq_t res = cache[i-1] * (seq_t)15;
            res -= cache[i-2] * (seq_t)95;
            res += cache[i-3] * (seq_t)330;
            res -= cache[i-4] * (seq_t)685;
            res += cache[i-5] * (seq_t)873;
            res -= cache[i-6] * (seq_t)685;
            res += cache[i-7] * (seq_t)330;
            res -= cache[i-8] * (seq_t)95;
            res += cache[i-9] * (seq_t)15;
            res -= cache[i-10];                                /* *1 */
            cache[i] = res;
        }
        cache_len = i + 1;
    }
    return cache[n];
}
