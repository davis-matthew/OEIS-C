/*
 * A214626  a(n) = (a(n-1) + a(n-3)) / gcd(a(n-1), a(n-3)) with a(0) = a(1) =
 *          1 and a(2) = 3.
 * https://oeis.org/A214626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 80, u32 n <= 164, u64 n <= 292,
 *            u128 n <= 705, big any n
 * Helpers:   gcd, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (a(n-1) + a(n-3)) / gcd(a(n-1), a(n-3))
   with a(0) = 1, a(1) = 1, a(2) = 3 */
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;    /* array holding computed values */
static idx_t cache_len = 0;    /* number of values currently stored */
static idx_t cache_cap = 0;    /* allocated capacity (in terms of values) */

/* Ensure that the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Allocate the cache if it does not exist yet. */
    if (cache == NULL) {
        /* Start with a small capacity that will grow as needed. */
        cache_cap = 4;
        cache = seq_alloc(cache_cap);
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* Grow capacity if necessary. */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;           /* double until large enough */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)3;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a3 = cache[i - 3];
            seq_t g  = gcd(a1, a3);
            cache[i] = (a1 / g) + (a3 / g);
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
