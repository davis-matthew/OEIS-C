/*
 * A199774  x-values in the solution to 17*x^2 + 16 = y^2.
 * https://oeis.org/A199774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-3) – a(n-6), a(1)=0, a(2)=3, a(3)=5, a(4)=32,
 *            a(5)=203, a(6)=333
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[6] = {0, 3, 5, 32, 203, 333};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Small indices are handled directly. */
    if (n <= 6ULL) return init[n - 1];

    /* Ensure that the first six terms are cached. */
    if (cache_cap < 6) {
        seq_t *tmp = (seq_t*)realloc(cache, 6 * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_cap = 6;
    }

    /* Extend the cache if we need more terms. */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_cap; i < n; ++i)
            cache[i] = 66 * cache[i - 3] - cache[i - 6];
        cache_cap = n;
    }

    return cache[n - 1];
}
