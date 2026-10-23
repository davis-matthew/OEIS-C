/*
 * A024166  a(n) = Sum_{1 <= i < j <= n} (j-i)^3.
 * https://oeis.org/A024166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + ((n*(n+1)/2)^2), a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_max = 0;   /* largest n for which cache[n] is stored */

seq_t a(idx_t n)
{
    /* Ensure that a(0) is available */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;               /* unlikely, but keep safe */
        cache[0] = 0;
        cached_max = 0;
    }

    if (n <= cached_max)
        return cache[n];

    /* Grow the cache to include up to index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                       /* memory allocation failed */
    cache = tmp;

    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t prod = (seq_t)k * (seq_t)(k + 1); /* k*(k+1) */
        prod /= 2;                            /* one factor is even */
        prod *= prod;                         /* square: (k*(k+1)/2)^2 */
        cache[k] = cache[k - 1] + prod;       /* recurrence */
    }

    cached_max = n;
    return cache[n];
}
