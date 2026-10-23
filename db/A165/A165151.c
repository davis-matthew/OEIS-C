/*
 * A165151  a(n) = (3*11^n - 7^n)/2.
 * https://oeis.org/A165151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*11^n - 7^n)/2 Recurrence: a(0)=1, a(1)=13,
 *            a(n)=18*a(n-1)-77*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Support memoisation across calls */
    static seq_t *cache = NULL;      /* array of pre‑computed terms */
    static idx_t max_cached = 0;     /* maximum index currently stored */
    static size_t cache_size = 0;    /* array capacity */

    /* initialise cache with the first two terms */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        max_cached = 1;
    }

    /* Return if already computed */
    if (n <= max_cached)
        return cache[(size_t)n];

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < ((size_t)n + 1))
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t term_prev  = cache[(size_t)(i - 1)];
        seq_t term_prev2 = cache[(size_t)(i - 2)];
        seq_t val = (seq_t)18 * term_prev - (seq_t)77 * term_prev2;
        cache[(size_t)i] = val;
    }
    max_cached = n;

    return cache[(size_t)n];
}
