/*
 * A121833  Expansion of 1/(1-x^2-x^3-x^6).
 * https://oeis.org/A121833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-6) Initial values: a(0) = 1, a(1) =
 *            0, a(2) = 1, a(3) = 1, a(4) = 1, a(5) = 2, a(6) = 3
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 68, u64 n <= 133,
 *            u128 n <= 264, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;      /* allocated array of seq_t */
static idx_t cache_size = 0;     /* current allocated size  */
static idx_t computed_upto = 0;  /* first index not yet computed */

/* Return the n-th term of A121833. */
seq_t a(idx_t n)
{
    /* Ensure the cache holds indices 0..n */
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        /* C-style cast allows the code to compile as both C and C++ */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed: return 0 as safe fallback */
        cache = new_cache;
        /* Initialise newly added slots to zero (default for seq_t is implementation‑defined) */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute missing values in increasing order */
    for (idx_t i = computed_upto; i <= n; ++i) {
        if (i == 0) {                    /* a(0) = 1 */
            cache[i] = 1;
            continue;
        }
        if (i == 1) {                    /* a(1) = 0 */
            cache[i] = 0;
            continue;
        }

        /* Recurrence using only indices that already exist */
        seq_t val = 0;
        if (i >= 2) val += cache[i - 2];
        if (i >= 3) val += cache[i - 3];
        if (i >= 6) val += cache[i - 6];
        cache[i] = val;
    }

    /* All indices ≤ n are now computed */
    if (computed_upto <= n)
        computed_upto = n + 1;

    return cache[n];
}
