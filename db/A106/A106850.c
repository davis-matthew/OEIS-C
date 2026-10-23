/*
 * A106850  Expansion of 3*x^2*(2*x^2+9*x-2)/((3*x^2-5*x+1)*(3*x^2+5*x-1)).
 * https://oeis.org/A106850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2) + 9*a(n-4) (for n ≥ 5) Initial
 *            values: a(1) = 0 a(2) = 6 a(3) = 33 a(4) = 174
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms already stored */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = seq_malloc(cache_size);
        cache[0] = (seq_t)0;    /* a(1) */
        cache[1] = (seq_t)6;    /* a(2) */
        cache[2] = (seq_t)33;   /* a(3) */
        cache[3] = (seq_t)174;  /* a(4) */
    }

    if (n <= cache_size)
        return cache[n - 1];

    /* grow the cache to accommodate term n */
    seq_t *new_cache = seq_malloc(n);
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term = new_cache[i - 2] * (seq_t)10;   /* 10*a(i-1) */
        term -= new_cache[i - 3] * (seq_t)25;       /* -25*a(i-2) */
        term += new_cache[i - 5] * (seq_t)9;        /* + 9*a(i-4) */
        new_cache[i - 1] = term;
    }

    cache = new_cache;
    cache_size = n;

    return cache[n - 1];
}
