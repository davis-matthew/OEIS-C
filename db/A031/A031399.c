/*
 * A031399  Numbers n with no 4k+3 factors such that the Pell equation x^2 - n
 *          y^2 = -1 is insoluble.
 * https://oeis.org/A031399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_no_3mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generates the OEIS sequence A031399, i.e. all
 * integers n that have no prime factor ≡ 3 (mod 4) and for which
 * the negative Pell equation x² − n y² = −1 has no integer solutions.
 * Each call produces a(n) (1‑based).  The sequence is built by
 * scanning successive integers, testing the two conditions, and
 * caching the results for fast subsequent lookup. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_no_3mod4.h"

/* Integer square‑root (unsigned 64‑bit) via Newton's method. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Return true iff the negative Pell equation x² − n y² = −1 has no integer solutions. */
static bool negative_pell_unsolvable(uint64_t n)
{
    uint64_t a0 = isqrt_u64(n);
    if (a0 * a0 == n)
        return true;                       /* perfect square → unsolvable */
    uint64_t m = 0, d = 1, a = a0;
    int period = 0;
    do {
        m = d * a - m;
        d = (n - m * m) / d;
        a = (a0 + m) / d;
        ++period;
    } while (a != 2 * a0);
    return (period % 2 == 0);
}

/* Main autoincrementing sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 3;   /* to start from 4 */

    if (n == 0)
        return (seq_t)0;   /* not defined, but harmless */

    /* If cache already contains the requested term, return it. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Grow the cache array if needed. */
    size_t needed = (size_t)n;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        /* cast realloc result for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return zero sentinel */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate further terms until the cache is long enough. */
    uint64_t cand = last_checked + 1;
    while (cache_len < needed) {
        if (has_no_3mod4(cand) && negative_pell_unsolvable(cand))
            cache[cache_len++] = (seq_t)cand;
        last_checked = cand;
        ++cand;
    }

    return cache[n - 1];
}
