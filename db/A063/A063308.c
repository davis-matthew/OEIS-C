/*
 * A063308  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          35 ).
 * https://oeis.org/A063308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 with a(2) = -1, a(3)
 *            = 25, a(4) = 70, a(5) = 112, a(6) = 154, a(7) = 196, a(8) = 238,
 *            a(9) = 280, a(10) = 322, a(11) = 364, a(12) = 406, a(13) = 452
 * Range:     i8 n <= 5, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Static cache for already-computed values */
static seq_t *cache = NULL;      /* pointer to the array of a(i) */
static size_t cache_capacity = 0; /* highest index currently stored (inclusive) */
static int initialized = 0;       /* flag whether cache has been initialised */

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 2 */
    if (n < 2) return 0;

    /* Initialise cache once with the first 12 non‑zero terms (indices 2..13) */
    if (!initialized) {
        size_t init_size = 14;               /* store indices 0..13 */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */

        /* zero‑initialise the array */
        for (size_t i = 0; i < init_size; ++i) cache[i] = 0;

        /* base values a(2) … a(13) */
        static const seq_t base[12] = {
            -1,  25,  70, 112, 154, 196,
            238, 280, 322, 364, 406, 452
        };
        for (idx_t i = 2; i <= 13; ++i)
            cache[i] = base[i - 2];

        cache_capacity = 13;        /* indices 0..13 stored */
        initialized = 1;
    }

    /* Grow cache and compute new values if a larger n is requested */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* reallocation failed */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = (idx_t)(cache_capacity + 1); i <= n; ++i) {
            if (i <= 13) {
                /* value already known from base array */
                continue;
            }
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        cache_capacity = new_cap;
    }

    return cache[n];
}
