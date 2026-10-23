/*
 * A138340  Expansion of (1-8*x)/(1-4*x+16*x^2).
 * https://oeis.org/A138340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A138340 (Expansion of (1-8*x)/(1-4*x+16*x^2))
 * Formula: a(n) = 4*a(n-1) - 16*a(n-2),  a(0)=1, a(1)=-4
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* dynamic array of computed terms */
static idx_t  cache_len = 0;           /* currently allocated size of cache   */
static idx_t  computed_until = 0;      /* highest index that has been filled */

/* Computes a(n) for any n >= 0, caching previously computed terms */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold index n */
    if (cache_len <= n) {
        idx_t new_len = (cache_len == 0) ? 2 : cache_len;
        while (new_len <= n) new_len <<= 1;          /* grow power‑of‑two */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failed */
        cache     = tmp;
        cache_len = new_len;
    }

    /* Seed base values if not already done */
    if (computed_until == 0) {
        cache[0] = (seq_t)1;
        if (cache_len >= 2) cache[1] = (seq_t)-4;
        computed_until = 1;
    }

    /* If the requested term is already computed, return it */
    if (n <= computed_until)
        return cache[n];

    /* Compute missing terms up to n, using the recurrence */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] - (seq_t)16 * cache[i-2];
    }
    computed_until = n;
    return cache[n];
}
