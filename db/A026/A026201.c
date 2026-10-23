/*
 * A026201  Position of n in A026200.
 * https://oeis.org/A026201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=5, a(4)=3, a(5)=9, a(6)=4, a(7)=13, a(8)=6;
 *            a(n)=a(n-2)+a(n-6)-a(n-8) for n>8
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cached_size = 0;
static idx_t last_computed = 0;

static const seq_t init[8] = {
    (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)3,
    (seq_t)9, (seq_t)4, (seq_t)13, (seq_t)6
};

seq_t a(idx_t n)
{
    /* Return precalculated small values */
    if (n <= 8) {
        return init[n - 1];
    }

    /* Ensure storage large enough to hold indices up to n */
    size_t needed = (size_t)n + 1;            /* indices 0 ... n */
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cached_size = needed;
    } else if (needed > cached_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cached_size = needed;
    }

    /* Initialize first 8 terms the first time the cache is built */
    if (last_computed < 8) {
        for (size_t i = 1; i <= 8 && i < cached_size; ++i) {
            cache[i] = init[i - 1];
        }
        last_computed = 8;
    }

    /* Compute new terms only once, even for out‑of‑order calls */
    for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 6] - cache[i - 8];
    }
    last_computed = (idx_t)n;

    return cache[n];
}
