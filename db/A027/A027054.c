/*
 * A027054  a(n) = T(n, n+3), T given by A027052.
 * https://oeis.org/A027054
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5) for n > 6 Initial
 *            values: a(3)=1, a(4)=8, a(5)=23, a(6)=52, a(7)=107, a(8)=210
 *            Sequence is defined for n >= 3
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t cache_cap = 0;          /* current allocated size (== capacity) */
static idx_t cache_max = 0;          /* largest index for which cache[index] is valid */

static inline void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = (n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3)           /* undefined below the offset – return 0 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = (idx_t)9;          /* indices 0..8 inclusive (2 unused) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)0;

        /* store the first six values */
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)23;
        cache[6] = (seq_t)52;
        cache[7] = (seq_t)107;
        cache[8] = (seq_t)210;
        cache_max = 8;
    }

    /* grow array if needed */
    ensure_capacity(n);

    /* if value already computed */
    if (n <= cache_max)
        return cache[n];

    /* compute missing values up to n (inclusive) */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term =   (seq_t)3 * cache[i - 1]
                     - (seq_t)2 * cache[i - 2]
                     -               cache[i - 4]
                     +               cache[i - 5];
        cache[i] = term;
    }
    cache_max = n;
    return cache[n];
}
