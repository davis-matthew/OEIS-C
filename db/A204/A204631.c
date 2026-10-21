/*
 * A204631  Expansion of 1/(1 - x - x^2 + x^5 - x^7).
 * https://oeis.org/A204631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) + a(n-7); Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=11
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 51, u64 n <= 102,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array for computed terms */
static idx_t cache_len = 0;      /* highest index computed + 1 */
static idx_t cache_cap = 0;      /* allocated capacity */

/* Base values for n = 0..6 */
static const seq_t BASE[7] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
    (seq_t)5, (seq_t)7, (seq_t)11
};

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n < 7) return BASE[n];

    /* initialise cache on first use */
    if (cache_len == 0) {
        /* Allocate space for up to n (but keep base values) */
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */
        for (idx_t i = 0; i < 7; ++i) cache[i] = BASE[i];
        cache_len = 7;
    }

    /* expand cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        /* Ensure we grow at least twice for efficiency */
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] - cache[i-5] + cache[i-7];
    }
    cache_len = n + 1;

    return cache[n];
}
