/*
 * A123958  Expansion of x^3 / ( 1+2*x^2+2*x^3 ).
 * https://oeis.org/A123958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 25, i32 n <= 49, i64 n <= 94, i128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123958: coefficients of the expansion of x^3 / (1+2*x^2+2*x^3) */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* cached coefficients a(0),a(1),… */
static size_t cache_size = 0;      /* number of elements stored (indices 0‑…‑cache_size-1) */

/* External linkage as requested */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise cache with first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;            /* unlikely, but safe */
        cache[0] = 0;                    /* coefficient of x^0 */
        cache[1] = 0;                    /* coefficient of x^1 */
        cache[2] = 0;                    /* coefficient of x^2 */
        cache[3] = 1;                    /* coefficient of x^3 */
    }

    /* Extend cache if necessary */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;

        /* Compute terms from cache_size up to idx */
        for (size_t i = cache_size; i <= idx; ++i) {
            if (i < 3) {
                cache[i] = 0;
            } else if (i == 3) {
                cache[i] = 1;
            } else {
                /* a(i) = -2*a(i-2) - 2*a(i-3)  (use additions only) */
                seq_t t1 = cache[i - 2];
                t1 += t1;                 /* 2*a(i-2) */
                t1 = -t1;                 /* -2*a(i-2) */
                seq_t t2 = cache[i - 3];
                t2 += t2;                 /* 2*a(i-3) */
                t2 = -t2;                 /* -2*a(i-3) */
                cache[i] = t1 + t2;
            }
        }
        cache_size = new_size;
    }

    return cache[idx];
}
