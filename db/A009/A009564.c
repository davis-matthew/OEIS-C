/*
 * A009564  E.g.f. sin(x^2)/2, coefficients of x^(4*n + 2).
 * https://oeis.org/A009564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (4*n+2)! / (2*(2*n+1)!)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 6, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Memoisation storage */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_cap = 0;    /* allocated capacity   */
static size_t cache_max = 0;    /* highest index cached */

/* Sequence term generator */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = 1;          /* a(0) = 1 */
        cache_max = 0;
    }

    /* Resize cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_max) {
        for (size_t k = cache_max + 1; k <= (size_t)n; ++k) {
            size_t i = k - 1;                 /* previous index          */
            uint64_t ii = (uint64_t)i;        /* for numeric operations  */

            /* numerator = (4i+3)(4i+4)(4i+5)(4i+6) */
            uint64_t num = (4*ii + 3ULL) *
                           (4*ii + 4ULL) *
                           (4*ii + 5ULL) *
                           (4*ii + 6ULL);

            /* denominator = (2i+2)(2i+3) */
            uint64_t den = (2*ii + 2ULL) *
                           (2*ii + 3ULL);

            /* division is exact by construction */
            uint64_t q = num / den;
            seq_t quotient = (seq_t)q;

            /* recurrence: a(i+1) = -a(i) * q */
            seq_t prev = cache[i];
            cache[k] = -prev * quotient;
        }
        cache_max = n;
    }

    return cache[n];
}
