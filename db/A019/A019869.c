/*
 * A019869  Expansion of 1/((1-5*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A019869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 162*a(n-2) + 360*a(n-3); a(0) = 1, a(1) = 23,
 *            a(2) = 367
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t full   = 0;

seq_t a(idx_t n) {
    /* Allocate initial cache with the first three terms */
    if (cache == NULL) {
        cap   = (size_t)3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));   /* cast for C++ */
        if (!cache) return (seq_t)0;                    /* fallback – should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)367;
        full = 2;
    }

    /* Expand cache size if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – should not happen */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms forward using the recurrence */
    for (idx_t i = full + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)23 * cache[(size_t)(i - 1)];
        seq_t t2 = (seq_t)162 * cache[(size_t)(i - 2)];
        seq_t t3 = (seq_t)360 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = t1 - t2 + t3;
    }

    /* Update the highest computed index */
    if (n > full) full = n;

    return cache[(size_t)n];
}
