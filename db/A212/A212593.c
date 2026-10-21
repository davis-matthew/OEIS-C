/*
 * A212593  a(n) is the difference between multiples of 9 with even and odd
 *          digit sum in base 8 in interval [0,8^n).
 * https://oeis.org/A212593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1..8) = {1,8,15,232,449,7400,14351,237832}; For n>=9: a(n) =
 *            36*a(n-2) - 126*a(n-4) + 84*a(n-6) - 9*a(n-8)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A212593. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed terms               */
static size_t cache_cap = 0;         /* allocated capacity of cache           */
static size_t cache_len = 0;         /* number of terms stored (max index+1)  */

/* Initial values for n = 1..8 */
static const seq_t init_av[8] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)15,
    (seq_t)232,
    (seq_t)449,
    (seq_t)7400,
    (seq_t)14351,
    (seq_t)237832
};

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    size_t target = (size_t)n;           /* we assume n fits in size_t (< 2^32) */

    /* Ensure cache has space */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
    } else if (cache_cap < target) {
        size_t new_cap = cache_cap;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute needed values */
    if (cache_len < target) {
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            size_t idx = (size_t)(i - 1); /* zero‑based index */

            if (i <= 8) {          /* initialise first eight terms */
                cache[idx] = init_av[idx];
            } else {                /* recurrence for i >= 9 */
                /* indices for a(i-2), a(i-4), a(i-6), a(i-8) */
                size_t i2 = idx - 2;
                size_t i4 = idx - 4;
                size_t i6 = idx - 6;
                size_t i8 = idx - 8;

                seq_t val = cache[i2] * (seq_t)36;
                val -= cache[i4] * (seq_t)126;
                val += cache[i6] * (seq_t)84;
                val -= cache[i8] * (seq_t)9;

                cache[idx] = val;
            }
        }
        cache_len = target;
    }

    return cache[n - 1];
}
