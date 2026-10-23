/*
 * A181982  Expansion of (1 - x^2)^2 * (1 + x)^2 / (1 - x^6) in powers of x.
 * https://oeis.org/A181982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2) - a(n-4) for n >= 6, with a(0) = 1, a(1) = 2,
 *            a(2) = -1, a(3) = -4, a(4) = -1, a(5) = 2
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values for fast repeated calls */
static seq_t *cache   = NULL;  /* array of computed a(n) */
static size_t  cap   = 0;      /* allocated capacity */
static size_t  len   = 0;      /* number of values stored */

/* initial six terms of the sequence */
static const seq_t init_terms[6] = {1, 2, -1, -4, -1, 2};

/* ensure the cache is large enough to hold values up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation – bigger than six to reduce resizes */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return;           /* allocation failed, leave empty */
        for (size_t i = 0; i < 6; ++i) cache[i] = init_terms[i];
        for (size_t i = 6; i < cap; ++i) cache[i] = 0;
        len = 6;
    }
    if ((size_t)n < len) return;   /* already computed */

    /* extend capacity if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;   /* reallocation failed – nothing more to compute */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = -(cache[i-2]) - (cache[i-4]);
    }
    len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n < 6) return init_terms[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
