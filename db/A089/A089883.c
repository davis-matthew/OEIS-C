/*
 * A089883  Expansion of (1-4*x+6*x^2-3*x^3)/((1-3*x)*(1-2*x)*(1-3*x+x^2)).
 * https://oeis.org/A089883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 23*a(n-3) - 6*a(n-4) for n>=4,
 *            initial values: a(0)=1, a(1)=4, a(2)=16, a(3)=60
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      // cache of computed values
static idx_t cache_size = 0;     // capacity of cache
static idx_t computed = 0;       // highest index computed (0 for none)

static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        /* initialise first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)60;
        computed = 3;
    }
    if (n < cache_size) return;     /* already enough capacity */

    /* grow capacity until it's > n */
    while (cache_size <= n) {
        size_t new_cap = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (n <= computed) return cache[n];

    /* compute new terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term_pos = 8 * cache[i-1] + 23 * cache[i-3];
        seq_t term_neg = 22 * cache[i-2] + 6 * cache[i-4];
        cache[i] = term_pos - term_neg;
    }
    computed = n;
    return cache[n];
}
