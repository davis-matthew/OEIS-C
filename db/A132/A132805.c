/*
 * A132805  A trisection of A024495.
 * https://oeis.org/A132805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 7*a(n) + 8*a(n-1), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence */
static idx_t seq_cached_size = 0;   /* number of elements stored (>=2) */
static size_t seq_cached_cap  = 0;   /* allocated capacity */
static seq_t *seq_cache      = NULL; /* pointer to cached values */

/* Expand the cache to hold at least elements 0..n */
static void seq_ensure_cache(idx_t n)
{
    if (seq_cached_size != 0) {
        if (n < seq_cached_size) return;           /* already have it */
    }

    /* Fast path to create initial cache */
    if (seq_cached_size == 0) {
        size_t init_cap = 2;                       /* need at least indices 0,1 */
        seq_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq_cache) exit(1); /* allocation failure */
        seq_cache[0] = 0;                    /* a(0) */
        seq_cache[1] = 3;                    /* a(1) */
        seq_cached_size = 2;
        seq_cached_cap  = init_cap;
        /* if requested n is 0 or 1 we are already done */
        if (n < 2) return;
    }

    /* Increase capacity if necessary */
    if ((size_t)n + 1 > seq_cached_cap) {
        size_t new_cap = seq_cached_cap ? seq_cached_cap << 1 : 2;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        seq_cached_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = seq_cached_size; i <= n; ++i) {
        if (i == 0) {
            seq_cache[i] = 0;
        } else if (i == 1) {
            seq_cache[i] = 3;
        } else {
            seq_cache[i] = 7 * seq_cache[i - 1] + 8 * seq_cache[i - 2];
        }
    }
    seq_cached_size = n + 1;
}

seq_t a(idx_t n)
{
    seq_ensure_cache(n);
    return seq_cache[n];
}
