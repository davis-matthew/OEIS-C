/*
 * A090400  Expansion of 1/(1-3x+3x^3) in powers of x.
 * https://oeis.org/A090400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) (Recurrence)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=3, a(2)=9  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;     /* array storing terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_filled = 0; /* number of terms that have been computed (cache_filled > 0 means cache[0..cache_filled-1] valid) */

    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap   = 4;                          /* initial capacity large enough for a(3) */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                      /* unrecoverable in contest environment */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache_filled = 3;                          /* terms 0..2 have been set */
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        do new_cap <<= 1;
        while (new_cap <= (size_t)n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms only up to n, if not already done */
    for (size_t i = cache_filled < 3 ? 3 : cache_filled; i <= (size_t)n; ++i) {
        cache[i] =  (seq_t)3 * cache[i - 1] - (seq_t)3 * cache[i - 3];
    }
    cache_filled = (size_t)n + 1;

    return cache[(size_t)n];
}
