/*
 * A052680  Expansion of e.g.f. (1-2*x)/(1-4*x+2*x^2).
 * https://oeis.org/A052680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(0) = 1, a(1) = 2 * a(n+2) = 4*(n+2)*a(n+1) -
 *            2*(2+3*n+n^2)*a(n) (n ≥ 0) * * This implementation memoises the
 *            sequence in a dynamic array, * allowing O(1) look‑up after the
 *            required terms have been computed. * It works for any seq_t type
 *            that supports +, -, *, / and for * a 64‑bit unsigned idx_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* stored values */
static size_t cache_cap  = 0;       /* allocated size */
static size_t cache_size = 0;       /* number of values already computed */

/* Ensure that cache[0..n] are valid. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;      /* we need entries 0..n */

    /* grow the allocated array if necessary */
    if (needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 128;
        while (newcap < needed)
            newcap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t *>(
                         realloc(cache, newcap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
#endif
        if (!tmp) exit(1);
        cache     = tmp;
        cache_cap = newcap;
    }

    /* initialise first two terms */
    if (cache_size == 0) {
        cache[0] = 1;
        cache_size = 1;
    }
    if (cache_size == 1 && needed > 1) {
        cache[1] = 2;
        cache_size = 2;
    }

    /* compute further terms using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t a_prev      = cache[i - 1];
        seq_t a_prev_prev = cache[i - 2];

        /* factor1 = 4 * i  (i = n+2 in the recurrence) */
        seq_t factor1 = (seq_t)i * 4;

        /* factor2 = 2 * (2 + 3*j + j^2), j = i-2 */
        idx_t j   = i - 2;
        idx_t tmp = 2 + 3 * j + j * j;          /* fits in 64‑bit for needed n */
        seq_t factor2 = (seq_t)(2 * tmp);

        /* a[i] = factor1 * a_prev - factor2 * a_prev_prev */
        cache[i] = a_prev * factor1 - a_prev_prev * factor2;
    }

    cache_size = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
