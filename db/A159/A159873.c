/*
 * A159873  Numerator of Hermite(n, 9/23).
 * https://oeis.org/A159873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 18 Recurrence: a(n) = 18*a(n-1) -
 *            1058*(n-1)*a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                      /* for size_t */
#include "../utils/seq_alloc_array.h"     /* seq_alloc_array(seq_t *, size_t) */
#include "../utils/seq_free_array.h"      /* seq_free_array(seq_t *) */

static seq_t *cache = NULL;      /* cached values up to 'computed' */
static idx_t  cache_cap = 0;     /* allocated size of the cache */
static idx_t  computed = 0;      /* highest index currently stored */

/* Ensure the cache contains a(0) … a(n). */
static void
ensure_cache(idx_t n)
{
    /* Grow array if required. */
    if (cache_cap <= n) {
        size_t new_cap = (n < 32 ? 32 : (size_t)n * 2 + 1);
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (cache) {
            for (idx_t i = 0; i <= computed; i++)
                new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Initialise base terms if this is the first run. */
    if (computed == 0) {
        cache[0] = (seq_t)1;           /* a(0) */
        computed = 0;
    }

    /* Compute a(1) if needed. */
    if (n >= 1 && computed < 1) {
        cache[1] = (seq_t)18;          /* a(1) */
        computed = 1;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)18;

        seq_t multiplier = (seq_t)1058 * (seq_t)(i - 1);
        seq_t t2 = cache[i - 2] * multiplier;

        cache[i] = t1 - t2;
    }
    computed = n;                     /* now all up to n are ready */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
