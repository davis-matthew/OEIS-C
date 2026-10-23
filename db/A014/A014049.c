/*
 * A014049  Inverse of 40th cyclotomic polynomial.
 * https://oeis.org/A014049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the coefficients of 1/(x^16 - x^12 + x^8 - x^4 + 1),
   which are periodic with period 40.
   a(0) = 1,   a(1) = a(2) = a(3) = 0.
   For n >= 4:
     a(n) = a(n-4) - a(n-8) + a(n-12) - a(n-16). */
#include <stdlib.h>
static seq_t *cache = NULL;
static idx_t cache_size  = 0;
static idx_t cache_capacity = 0;

static void ensure_capacity(idx_t needed)
{
    if (needed < cache_capacity) return;
    idx_t new_cap = cache_capacity ? cache_capacity * 2 : 32;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* allocation failure – terminate */
    if (cache)
    {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_capacity = new_cap;
}

static void init_cache(void)
{
    if (!cache)
    {
        cache_capacity = 32;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1   */
        cache_size  = 1;       /* next index to fill is 1 */
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n >= cache_size)
    {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i)
        {
            seq_t v = 0;
            if (i >= 4)  v += cache[i - 4];
            if (i >= 8)  v -= cache[i - 8];
            if (i >= 12) v += cache[i - 12];
            if (i >= 16) v -= cache[i - 16];
            cache[i] = v;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
