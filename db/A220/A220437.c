/*
 * A220437  Number of domino tilings of a cross whose center is a 4 X 4 square
 *          and in which each of the four arms has length n.
 * https://oeis.org/A220437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1}^{9} d_k * a(n-k) with d = [5, 35, -67, -145, 145,
 *            67, -35, -5, 1]. Initial values: a(0)=6, a(1)=28, a(2)=340,
 *            a(3)=2304, a(4)=20652, a(5)=157926, a(6)=1313248
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeff[10] = { -1, 5, 35, -67, -145, 145, 67, -35, -5, 1 };
static const seq_t init[7] = {6U, 28U, 340U, 2304U, 20652U, 157926U, 1313248U};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure that the cache has room for (n+1) elements. */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? needed : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Allocate or extend the cache. */
    ensure_cache_capacity(n);

    /* Fill in initial known terms if the cache is empty. */
    if (cache_len == 0) {
        size_t first = (n < 6U) ? (size_t)n + 1 : 7;
        for (size_t i = 0; i < first; ++i) cache[i] = init[i];
        cache_len = first;
    }

    /* Compute missing terms using the linear recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 7) {
            cache[i] = init[i];
            continue;
        }
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 1; k <= 9; ++k) {
            if (i >= (size_t)k) {
                seq_t prod = (coeff[k] >= 0 ?
                              (seq_t)coeff[k] * cache[i - k] :
                              (seq_t)(-coeff[k]) * cache[i - k]);
                if (coeff[k] >= 0) sum_pos += prod;
                else               sum_neg += prod;
            }
        }
        cache[i] = sum_pos - sum_neg;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
