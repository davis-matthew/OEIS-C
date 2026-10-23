/*
 * A015574  Expansion of x/(1 - 8*x - 3*x^2).
 * https://oeis.org/A015574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;          /* actual capacity of seq_cache */
static idx_t cache_max = 1;           /* largest n for which a(n) has been computed */

/* Grow the cache so that it can hold at least index n0 (inclusive) */
static void ensure_cache(idx_t n0)
{
    if (seq_cache == NULL) {
        /* allocate at least two slots for a(0) and a(1) */
        cache_cap = n0 < 2 ? 2 : (size_t)n0 + 1;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
        for (size_t i = 0; i < cache_cap; ++i) seq_cache[i] = (seq_t)0;
    }
    else if ((idx_t)cache_cap <= n0) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n0) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) seq_cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Ensure the initial terms are set */
    seq_cache[0] = (seq_t)0;
    seq_cache[1] = (seq_t)1;
}

/* Compute a(n) using memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);

    /* Generate values up to n if needed */
    if (cache_max < (idx_t)n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_cache[i] = (seq_t)8 * seq_cache[i-1] + (seq_t)3 * seq_cache[i-2];
        }
        cache_max = n;
    }
    return seq_cache[n];
}
