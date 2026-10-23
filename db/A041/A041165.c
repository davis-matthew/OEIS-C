/*
 * A041165  Denominators of continued fraction convergents to sqrt(92).
 * https://oeis.org/A041165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2302*a(n-8) - a(n-16) (n >= 16) Initial values (n =
 *            0..15): 1, 1, 2, 5, 22, 49, 71, 120, 2231, 2351, 4582, 11515,
 *            50642, 112799, 163441, 276240
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Raw initial values stored as 64‑bit constants so that the compiler
   does not overflow when building for small seq_t sizes.  They are
   converted to seq_t only when accessed. */
static const uint64_t init_raw[16] = {
    1ULL, 1ULL, 2ULL, 5ULL, 22ULL, 49ULL, 71ULL, 120ULL,
    2231ULL, 2351ULL, 4582ULL, 11515ULL, 50642ULL,
    112799ULL, 163441ULL, 276240ULL
};

static seq_t *cache = NULL;      /* array storing computed terms */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure that cache contains entries up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < 16) {
        /* All values required are in init_raw, so nothing to compute. */
        return;
    }

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* Copy the 16 initial terms into the cache. */
        for (size_t i = 0; i < 16; ++i) {
            cache[i] = (seq_t)init_raw[i];
        }
        cache_len = 16;
    }

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t a8  = cache[i - 8];
        seq_t a16 = cache[i - 16];
        cache[i] = ((seq_t)2302) * a8 - a16;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n < 16) {
        return (seq_t)init_raw[(size_t)n];
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
