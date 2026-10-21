/*
 * A051560  Second unsigned column of triangle A051379.
 * https://oeis.org/A051560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+13)*a(n-1) - (n+6)^2*a(n-2), a(0) = 0, a(1) = 1 (for
 *            n >= 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recursive linear recurrence with polynomial coefficients.  */
#include <stdlib.h>

/* Cached values of the sequence.  */
static seq_t *cache = NULL;      /* buffer holding a(0) .. a(k) */
static idx_t  cache_len = 0;     /* number of entries currently stored */
static size_t cache_cap = 0;     /* malloced capacity */

/* Ensure that cache contains values up to index n (inclusive).  */
static void ensure_to_n(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    if (!cache)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* Initialise new entries to zero – required if seq_t is a class. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;

    cache_cap = new_cap;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t coef1 = (seq_t)(2 * i + 13);
            seq_t coef2 = (seq_t)((i + 6) * (i + 6));
            cache[i] = coef1 * a1 - coef2 * a2;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_to_n(n);
    return cache[n];
}
