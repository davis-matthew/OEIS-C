/*
 * A124395  Expansion of (1-2*x)/(1-2*x+2*x^3).
 * https://oeis.org/A124395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0
 * Range:     i8 n <= 11, i16 n <= 26, i32 n <= 50, i64 n <= 103,
 *            i128 n <= 205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If this compilation target has a seq_t type that is not a primitive,
   the explicit cast from void* ensures the pointers are valid.
   The objects are constructed only by assignment (which is adequate for
   the provided arbitrary‑precision integer types). */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (number of elements) */
static size_t cache_len = 0;   /* next index to compute (i.e. one past the
                                 last known value) */

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    /* first call: allocate base capacity */
    if (cache == NULL) {
        cache_cap = 8;
        while (cache_cap <= (size_t)n + 1) cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        /* initialize first three values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache_len = 3;   /* indices 0..2 are computed */
        return;
    }

    /* grow as needed */
    if (cache_cap <= (size_t)n + 1) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) = 2*a(n-1) - 2*a(n-3) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;

    ensure_cache(n);

    /* compute forward from the last known index */
    size_t start = cache_len;          /* one past last computed value */
    if (start < 3) start = 3;
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t val1 = cache[i - 1];
        seq_t val3 = cache[i - 3];     /* i >= 3 guaranteed */
        cache[i] = 2 * val1 - 2 * val3;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
