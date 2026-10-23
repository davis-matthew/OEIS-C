/*
 * A041957  Denominators of continued fraction convergents to sqrt(501).
 * https://oeis.org/A041957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; for n >= 1: a(n) = coeff[(n-1)%28] * a(n-1) + a(n-2)
 *            where coeff =
 *            {2,1,1,1,1,3,8,1,2,10,1,5,2,14,2,5,1,10,2,1,8,3,1,1,1,1,2,44}
 *            (coefficients are the continued–fraction terms of √501 after the
 *            initial a0=22)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t size  = 0;   /* number of stored terms, cache[0]..cache[size-1] */

/* periodic continued‑fraction coefficients for √501 after a0=22 */
static const int cf_coeff[28] = {
    2, 1, 1, 1, 1, 3, 8, 1, 2, 10, 1, 5, 2, 14, 2, 5,
    1, 10, 2, 1, 8, 3, 1, 1, 1, 1, 2, 44
};

static void ensure_cache(idx_t n)
{
    if (size > n) return;          /* already have value up to n */

    /* grow storage if necessary */
    if (cap <= n) {
        if (cap == 0) cap = 16;
        while (cap <= n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* initialise first term if not yet done */
    if (size == 0) {
        cache[0] = 1;
        size = 1;
    }

    /* compute new terms up to n */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        seq_t prev1 = cache[i-1];
        seq_t prev2 = (i >= 2) ? cache[i-2] : 0;
        int coeff   = cf_coeff[(i-1) % 28];
        cache[i] = (seq_t)coeff * prev1 + prev2;
    }
    size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
