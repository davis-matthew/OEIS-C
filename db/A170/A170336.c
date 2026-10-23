/*
 * A170336  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 39, 1482, 56316, 2140008, thereafter a(n) = 38 *
 *            a(n-1). The sequence is geometric from n = 4 onwards
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached a(n) values */
static idx_t  cache_len = 0;         /* number of values stored */

/* Small initial values (indices 0..4) */
static const seq_t init_vals[5] = {
    (seq_t)1,          /* a(0) */
    (seq_t)39,         /* a(1) */
    (seq_t)1482,       /* a(2) */
    (seq_t)56316,      /* a(3) */
    (seq_t)2140008     /* a(4) */
};

/* Grow the cache to hold values up to 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)      /* already computed */
        return;

    /* Increase capacity when needed. */
    size_t new_len = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    }

    /* Fill new entries. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 5) {
            cache[i] = init_vals[i];
        } else {
            /* a(i) = 38 * a(i-1) */
            cache[i] = cache[i-1] * (seq_t)38;
        }
    }
    cache_len = (idx_t)new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
