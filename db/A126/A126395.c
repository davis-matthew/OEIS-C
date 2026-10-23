/*
 * A126395  Number of base 8 n-digit numbers with adjacent digits differing by
 *          two or less.
 * https://oeis.org/A126395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 6*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 8, a(2) = 34, a(3) = 150
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* storage for computed terms */
static idx_t  computed = 0;   /* number of terms stored (indices 0..computed-1) */

/* Ensure that terms up to n have been computed and stored in cache */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* first allocation: at least 4 terms for the recurrence base cases */
        idx_t sz = n + 1;
        if (sz < 4) sz = 4;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
    } else if (computed <= n) {
        /* grow the allocation */
        idx_t sz = n + 1;
        cache = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    }

    /* Compute any missing terms */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 8;
        } else if (i == 2) {
            cache[2] = 34;
        } else if (i == 3) {
            cache[3] = 150;
        } else {
            seq_t term = 5 * cache[i - 1];
            term -= cache[i - 2];
            term -= 6 * cache[i - 3];
            cache[i] = term;
        }
    }
    computed = n + 1;   /* we now have terms 0..n inclusive */
}

/* Public entry point for the sequence */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
