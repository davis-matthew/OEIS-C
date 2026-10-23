/*
 * A161713  a(n) = (-n^5 + 15*n^4 - 65*n^3 + 125*n^2 - 34*n + 40)/40.
 * https://oeis.org/A161713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-n^5 + 15*n^4 - 65*n^3 + 125*n^2 - 34*n + 40)/40
 *            Computed via the linear recurrence: a(n) = 6*a(n-1) - 15*a(n-2)
 *            + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6) with initial values
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(4)=14, a(5)=28
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 156, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Computes the n-th term of OEIS A161713. */
seq_t a(idx_t n)
{
    /* Initialise cache for the first six values. */
    if (cache_size <= 5) {
        idx_t new_size = 6;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failed, return 0 as fallback */
            cache = tmp;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)28;
        cache_size = new_size;
    }

    /* Return cached value if already computed. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Extend cache up to n and compute new terms via recurrence. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return 0; /* allocation failed */
    cache = tmp;
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1]
                 - 15 * cache[i - 2]
                 + 20 * cache[i - 3]
                 - 15 * cache[i - 4]
                 + 6 * cache[i - 5]
                 - cache[i - 6];
    }
    cache_size = new_size;
    return cache[n];
}
