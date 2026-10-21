/*
 * A083707  G.f.: (x+4*x^3+x^5)/((1-x)^2*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A083707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+a(n-2)-3*a(n-3)-a(n-4)+a(n-5)+3*a(n-6)-a(n-7)-2*a(n-8)+a(n-9)
 *            with a(0)=0, a(1)=1, a(2)=2, a(3)=9, a(4)=17, a(5)=37, a(6)=63,
 *            a(7)=108, a(8)=165
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms already computed */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure the cache can hold at least 'need' elements, expanding if necessary. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first nine terms of the sequence. */
static void init_cache(void)
{
    static const seq_t base[9] = {
        0, 1, 2, 9, 17, 37, 63, 108, 165
    };
    grow_cache(9);
    for (size_t i = 0; i < 9; ++i)
        cache[i] = base[i];
    cache_len = 9;
}

/* Compute a(n) using the recurrence and memoise the result. */
seq_t a(idx_t n)
{
    if (n < 0)
        return 0; /* undefined for negative indices */

    if (cache_len == 0)
        init_cache();

    /* Extend the cache up to n if necessary. */
    if ((size_t)n >= cache_len) {
        grow_cache((size_t)n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* For indices below 9, the base terms are already in cache. */
            seq_t term = 0;
            term = 2 * cache[i-1] + cache[i-2]
                 - 3 * cache[i-3] - cache[i-4]
                 + cache[i-5] + 3 * cache[i-6]
                 -      cache[i-7] - 2 * cache[i-8]
                 +       cache[i-9];
            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
