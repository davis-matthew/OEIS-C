/*
 * A127113  n! in base 6.
 * https://oeis.org/A127113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal number obtained by writing n! in base‑6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   factorial_to_base6_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial_to_base6_decimal.h"

/* Static cache state */
static seq_t *cache = NULL;        /* array of computed a(n) values */
static unsigned char *seen = NULL; /* 1 if cache[n] is valid, 0 otherwise */
static size_t cache_size = 0;      /* current allocated array size */
static idx_t last_index = 0;       /* highest n for which factorial is known */
static seq_t last_factor = (seq_t)1;/* factorial of last_index */

/* Ensure the cache is allocated for at least one element. */
static void init_cache(void)
{
    if (cache_size == 0) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        seen  = (unsigned char *)calloc(1, sizeof(unsigned char));
        cache_size = 1;
        seen[0] = 1;
        cache[0] = (seq_t)1;  /* a(0) = 1 (0! = 1) */
    }
}

/* Convert a factorial value (in decimal) to the desired a(n). */

/* Extend the cache upwards to n, computing factorial incrementally. */
static seq_t cache_greater(idx_t n)
{
    /* Grow arrays if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        seen  = (unsigned char *)realloc(seen,  new_size * sizeof(unsigned char));
        /* Initialise new entries as uncomputed */
        for (size_t i = cache_size; i < new_size; ++i) {
            seen[i] = 0;
        }
        cache_size = new_size;
    }

    /* Compute factorial up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        last_factor = last_factor * (seq_t)i;
    }
    last_index = n;

    /* Convert and store */
    cache[n] = factorial_to_base6_decimal(last_factor);
    seen[n] = 1;
    return cache[n];
}

/* Compute a(n) from scratch (used when n <= last_index but not cached). */
static seq_t compute_from_scratch(idx_t n)
{
    /* Grow arrays if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        seen  = (unsigned char *)realloc(seen,  new_size * sizeof(unsigned char));
        for (size_t i = cache_size; i < new_size; ++i) {
            seen[i] = 0;
        }
        cache_size = new_size;
    }

    /* Factorial from 1 to n */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact = fact * (seq_t)i;
    }

    /* Convert, cache, and return */
    cache[n] = factorial_to_base6_decimal(fact);
    seen[n] = 1;
    return cache[n];
}

/* Main sequence function required by the framework */
seq_t a(idx_t n)
{
    init_cache();

    /* Return cached value if available */
    if (n < cache_size && seen[n]) {
        return cache[n];
    }

    /* Compute depending on position relative to last_index */
    if (n > last_index) {
        return cache_greater(n);
    } else {
        return compute_from_scratch(n);
    }
}
