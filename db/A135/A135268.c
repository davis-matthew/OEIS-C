/*
 * A135268  a(0) = 1; a(n) = a(n-1) + n - gcd(n,a(n-1)).
 * https://oeis.org/A135268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + n - gcd(n,a(n-1)) with a(0) = 1 Where gcd is the
 *            integer greatest common divisor
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Memoization cache */
static seq_t *cache = NULL;       /* array of calculated values   */
static idx_t  cache_len = 0;      /* number of valid entries      */
static size_t cache_cap = 0;      /* allocated capacity (entries) */

/* ensure cache has space for indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n + 1) {
        new_cap *= 2;
    }

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In case of allocation failure we abort; the build system
           assumes memory is available for this simple cache. */
        exit(EXIT_FAILURE);
    }

    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle base case first */
    if (n == 0) {
        /* Ensure cache is initialized for a(0) */
        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Extend cache if necessary */
    if (n >= cache_len) {
        ensure_capacity(n);

        /* Compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t g    = gcd((seq_t)i, prev);
            cache[i] = prev + (seq_t)i - g;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
