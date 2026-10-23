/*
 * A160090  Positive numbers y such that y^2 is of the form x^2 + (x + 569)^2
 *          with integer x.
 * https://oeis.org/A160090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; a(1)=485, a(2)=569,
 *            a(3)=689, a(4)=2221, a(5)=2845, a(6)=3649
 * Range:     u8 none, u16 n <= 9, u32 n <= 29, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;         /* 1‑based cache array */
static size_t cache_size = 0;       /* allocated slots, >=1 */
static idx_t max_computed = 0;      /* highest index already filled */

/* make sure cache has room for index n and that a(1)…a(n) are computed */
static void ensure_cache(idx_t n) {
    /* allocate / grow the cache */
    if (cache_size < (size_t)n + 1) {
        size_t new_size = (size_t)n + 10;          /* extra space to reduce reallocs */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        /* zero‑initialize new slots (important for big‑int types) */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        cache      = new_cache;
        cache_size = new_size;
    }

    /* initial values for indices 1 … 6 */
    static const seq_t init[7] = {0, 485, 569, 689, 2221, 2845, 3649};

    idx_t start = max_computed + 1;
    if (start < 1) start = 1;
    if (start < 6) start = 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else if (i > 6) {
            cache[i] = 6 * cache[i - 3] - cache[i - 6];
        }
    }
    if (n > max_computed) max_computed = n;
}

/* PUBLIC API ----------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 gracefully */
    ensure_cache(n);
    return cache[n];
}
