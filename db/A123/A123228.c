/*
 * A123228  Sum of the n-th powers of the roots of the polynomial x^6 + 14*x^5
 *          + 87*x^4 + 148*x^3 + 87*x^2 + 14*x + 1.
 * https://oeis.org/A123228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -13*a(n-1) - 74*a(n-2) - 74*a(n-3) - 13*a(n-4) - a(n-5)
 *            for n ≥ 5 with initial values a(0)=6, a(1)=-14, a(2)=22,
 *            a(3)=466, a(4)=-6714
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure that cache contains all terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Ensure enough capacity (at least 5 slots for the base terms). */
    size_t needed = (size_t)n + 1;
    if (needed < 5) needed = 5;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first five terms if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)-14;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)466;
        cache[4] = (seq_t)-6714;
        cache_len = 5;
    }

    /* If we already have a value for n, nothing more to do. */
    if (cache_len > (size_t)n)
        return;

    /* Compute missing terms using the recurrence relation. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)-13 * cache[i-1]
                 + (seq_t)-74 * cache[i-2]
                 + (seq_t)-74 * cache[i-3]
                 + (seq_t)-13 * cache[i-4]
                 + (seq_t)-1  * cache[i-5];
    }
    cache_len = (size_t)n + 1;
}

/* External linkage function requested by the judge. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
