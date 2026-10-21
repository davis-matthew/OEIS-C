/*
 * A081046  Difference of the first two Stirling numbers of the first kind.
 * https://oeis.org/A081046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, changed
 * Formula:   a(n) + (2*n-1)*a(n-1)+(n-1)^2*a(n-2) = 0 with a(0) = 1, a(1) =
 *            -2
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t    cache_size = 0;      /* number of valid entries */
static idx_t    cache_capacity = 0;  /* allocated size */

/* Ensure that terms up to n are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Allocate or enlarge the cache */
    if (n >= cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* In a real program you might abort or handle this error
               differently; for the purposes of this sequence we simply
               reallocate via malloc again. */
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)-2;
        } else {
            seq_t coef1 = (seq_t)(2 * (seq_t)i - 1);
            seq_t coef2 = (seq_t)(i - 1);
            seq_t term1 = coef1 * cache[i - 1];
            seq_t term2 = coef2 * coef2 * cache[i - 2];
            cache[i] = -(term1 + term2);
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
