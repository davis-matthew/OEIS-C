/*
 * A109785  Expansion of (1+x+x^2+x^7+x^8-2*x^10-x^12) /
 *          ((x+1)*(x^2+1)*(x^2+x+1)*(x^2-x+1)*(x^4-x^2+1)*(x-1)^2).
 * https://oeis.org/A109785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-12) - a(n-13) for n>12, with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=3, a(4)=3, a(5)=3, a(6)=3, a(7)=4,
 *            a(8)=5, a(9)=5, a(10)=3, a(11)=3, a(12)=3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 2, 3, 3, 3, 3, 3, 4, 5, 5, 3, 3, 3
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* initial capacity large enough for at least the first 13 terms */
    cache_cap = 32;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        exit(1);
    cache_size = 13;
    for (idx_t i = 0; i < 13; ++i)
        cache[i] = init_vals[i];
}

static void expand_cache(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_size)
        return cache[n];

    expand_cache(n);
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i <= 12)           /* safety, though not needed after init */
            cache[i] = init_vals[i];
        else
            cache[i] = cache[i - 1] + cache[i - 12] - cache[i - 13];
    }
    cache_size = n + 1;
    return cache[n];
}
