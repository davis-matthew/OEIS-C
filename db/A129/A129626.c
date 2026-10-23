/*
 * A129626  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+281)^2 = y^2.
 * https://oeis.org/A129626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 562 for n > 6 a(1)=0, a(2)=76,
 *            a(3)=559, a(4)=843, a(5)=1239, a(6)=3976
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th element (n ≥ 1) of OEIS A129626. */
seq_t a(idx_t n)
{
    /* static cache of computed values; index 0 unused.
       cache[i] holds a(i) for i >= 1 up to cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* highest index computed */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 7;               /* store indices 0..6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                  /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)76;
        cache[3] = (seq_t)559;
        cache[4] = (seq_t)843;
        cache[5] = (seq_t)1239;
        cache[6] = (seq_t)3976;
        cache_size = 6;
    }

    if (n <= cache_size)            /* already computed */
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – fall back to recomputing from scratch */
            /* this should never happen in typical use */
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;
            /* reinitialise */
            new_cache[0] = 0;
            new_cache[1] = (seq_t)0;
            new_cache[2] = (seq_t)76;
            new_cache[3] = (seq_t)559;
            new_cache[4] = (seq_t)843;
            new_cache[5] = (seq_t)1239;
            new_cache[6] = (seq_t)3976;
            cache = new_cache;
        } else {
            cache = new_cache;
        }
        cache_cap = new_cap;
    }

    /* compute up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = (seq_t)6 * cache[i - 3] - cache[i - 6] + (seq_t)562;
        cache[i] = val;
    }
    cache_size = n;

    return cache[n];
}
