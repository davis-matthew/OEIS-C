/*
 * A107087  G.f. A(x) satisfies: A(x)^2 = A(x^2) + 4*x.
 * https://oeis.org/A107087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=2. For n ≥ 2: 2·a(n) + Σ_{i=1}^{n-1} a(i)·a(n‑i) =
 *            (n even ? a(n/2) : 0)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 21, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* table of already‑computed terms */
static idx_t cache_size = 0;     /* allocated capacity of cache      */
static idx_t computed = 0;       /* largest index that has been filled */

seq_t a(idx_t n)
{
    /* Initialise the cache with the base values a(0)=1, a(1)=2 */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                /* fatal if allocation fails */
        cache[0] = 1;
        cache[1] = 2;
        computed = 1;                      /* indices 0 and 1 are ready */
    }

    if (n <= computed)
        return cache[n];

    /* Grow the cache if the requested index is beyond its current size */
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all new terms up to index n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* For k ≥ 2 the following formula applies */
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i)
            sum += cache[i] * cache[k - i];

        seq_t rhs = (k % 2 == 0) ? cache[k / 2] : 0;
        cache[k] = (rhs - sum) / 2;
    }
    computed = n;
    return cache[n];
}
