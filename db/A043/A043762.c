/*
 * A043762  Number of runs in the base 2 representation of n is congruent to 8
 *          mod 9.
 * https://oeis.org/A043762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose binary representation has
 *            a number of runs congruent to 8 modulo 9
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Count the runs in the binary expansion of x. */
static int count_runs(idx_t x) {
    int runs = 0;
    int prev = -1;
    while (x) {
        int bit = (int)(x & 1);
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        x >>= 1;
    }
    return runs;
}

/* Cache of the first N terms encountered so far. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* The largest integer tested so far. */
static idx_t last_tested = 0;

seq_t a(idx_t n) {
    if (n == 0) {
        /* Alphabetical: a(1) is defined, a(0) is ignored. */
        return (seq_t)0;
    }

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        /* Test the next integer. */
        ++last_tested;

        /* Accept it if the number of runs is 8 mod 9. */
        if ((count_runs(last_tested) % 9) == 8) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Out of memory: abort. */
                    abort();
                }
                if (cache) {
                    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)last_tested;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
