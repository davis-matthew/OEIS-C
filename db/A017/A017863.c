/*
 * A017863  Expansion of 1/(1-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)
 *            for n≥14. Initial values: a(0)=1; a(1)=0; a(2)=0; a(3)=0;
 *            a(4)=0; a(5)=0; a(6)=0; a(7)=1; a(8)=1; a(9)=1; a(10)=1;
 *            a(11)=1; a(12)=1; a(13)=1
 * Range:     u8 n <= 37, u16 n <= 63, u32 n <= 117, u64 n <= 223,
 *            u128 n <= 435, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cached sequence values */
static idx_t cache_size = 0;          /* highest index stored (exclusive) */

static const seq_t init_vals[14] = {
    1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1
};

seq_t a(idx_t n)
{
    /* Ensure at least the first 14 values are cached. */
    if (cache_size == 0) {
        cache_size = 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;   /* out‑of‑memory: return zero (sequence underflow) */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[(size_t)i] = init_vals[i];
    }

    /* Return a cached value if available. */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Grow cache to accommodate new index. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;               /* out‑of‑memory: return zero */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 7; k <= 14; ++k)
            sum += cache[(size_t)(i - k)];
        cache[(size_t)i] = sum;
    }

    cache_size = (idx_t)n + 1;
    return cache[(size_t)n];
}
