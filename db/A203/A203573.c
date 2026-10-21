/*
 * A203573  Bisection of A099924 (convolution of Lucas numbers); even
 *          arguments.
 * https://oeis.org/A203573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4); with a(0)=4,
 *            a(1)=13, a(2)=45, a(3)=152
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_cap = 0;       /* allocated capacity (number of elements) */
static idx_t cache_len = 0;        /* index of one past the largest computed term */

/* Ensure memory enough to store index 'n' */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute terms up to index 'n' if not already done */
static void ensure_len(idx_t n)
{
    if (cache_len > n)
        return;

    ensure_capacity(n);

    /* Initialise the first four terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        if (cache_cap > 1) cache[1] = (seq_t)13;
        if (cache_cap > 2) cache[2] = (seq_t)45;
        if (cache_cap > 3) cache[3] = (seq_t)152;
    }

    /* Compute further terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4)     /* base values already set */
            continue;

        seq_t term1 = (seq_t)(6 * cache[i-1] - (seq_t)11 * cache[i-2]);
        seq_t term2 = (seq_t)(6 * cache[i-3] - cache[i-4]);
        cache[i] = term1 + term2;
    }

    cache_len = n + 1;
}

/* Public interface required by the test harness */
seq_t a(idx_t n)
{
    ensure_len(n);
    return cache[n];
}
