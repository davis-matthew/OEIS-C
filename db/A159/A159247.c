/*
 * A159247  Numerator of Hermite(n, 1/10).
 * https://oeis.org/A159247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 1, a(n) = 50 * (1 - n) * a(n-2) + a(n-1) for n
 *            > 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use only the standard library */
#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache       = NULL;   /* array of computed values   */
static size_t cache_cap   = 0;      /* allocated size             */
static size_t cache_len   = 0;      /* number of filled entries   */

/* Simple capacity‑ensuring helper (C++17 compatible) */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure is fatal */

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute all terms up to and including n */
static void ensure_up_to(idx_t n)
{
    if ((size_t)n < cache_len) return;      /* already computed */

    ensure_capacity((size_t)n + 1);         /* need space for indices 0..n */

    /* Initialise first terms if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Fill missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t coeff = (seq_t)50 * ((seq_t)1 - (seq_t)i);   /* 50*(1-i) */
        cache[i] = prev1 + coeff * prev2;
    }

    cache_len = (size_t)n + 1;
}

/* External function demanded by the build */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
