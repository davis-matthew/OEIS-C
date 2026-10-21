/*
 * A214332  Trajectory of 0 under the morphism 0 -> 01, 1 -> 202, 2 -> empty
 *          string.
 * https://oeis.org/A214332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th symbol of the infinite word produced by the
 *            morphism 0 -> 01, 1 -> 202, 2 -> ε (empty string)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Global cache containing the computed prefix of the infinite word. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache has at least n + 1 symbols. */
static void ensure_cache(idx_t n)
{
    /* Initialisation: start with the word "0". */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
    }

    /* Iteratively apply the morphism until we have enough symbols. */
    while (cache_len <= (size_t)n) {
        /* Compute length of the next word. */
        size_t new_len = 0;
        for (idx_t i = 0; i < (idx_t)cache_len; ++i) {
            seq_t v = cache[i];
            if (v == 0)          new_len += 2; /* 0 -> 01  */
            else if (v == 1)     new_len += 3; /* 1 -> 202 */
            /* 2 -> ε  contributes 0 */
        }
        if (new_len == 0) break;   /* no more symbols can be produced */

        /* Allocate space for the new word and build it. */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;
        for (idx_t i = 0; i < (idx_t)cache_len; ++i) {
            seq_t v = cache[i];
            if (v == 0) {
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
            } else if (v == 1) {
                new_cache[pos++] = 2;
                new_cache[pos++] = 0;
                new_cache[pos++] = 2;
            } /* v == 2: nothing added */
        }

        /* Replace old word by the new one. */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
}

/* Sequence function required by the specification. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
