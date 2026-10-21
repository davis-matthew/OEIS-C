/*
 * A212902  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          |w-x|<|x-y|<|y-z|.
 * https://oeis.org/A212902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212902
   a(n) = 2*a(n-1)+a(n-2)-3*a(n-3)-a(n-4)+a(n-5)+3*a(n-6)-a(n-7)-2*a(n-8)+a(n-9)
   Initial terms: a(0)=0, a(1)=0, a(2)=2, a(3)=14, a(4)=44, a(5)=110,
   a(6)=228, a(7)=426, a(8)=726
 */
#include <stdlib.h>

static const seq_t base[9] = {0, 0, 2, 14, 44, 110, 228, 426, 726};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache of computed values   */
    static idx_t cache_size = 0;  /* number of entries currently valid */

    /* size needed for this call (at least n+1, but also at least 9 for base) */
    size_t needed = (size_t)n + 1;
    if (needed < 9) needed = 9;

    /* keep old cache size for later computation */
    size_t old_size = (size_t)cache_size;

    /* extend the cache if required */
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                 /* out‑of‑memory */
        cache = tmp;
        for (size_t i = old_size; i < needed; ++i) {
            if (i < 9) cache[i] = base[i];
            else cache[i] = 0;
        }
        cache_size = (idx_t)needed;
    }

    /* if the requested value is already computed, return it */
    if (n < cache_size - 1) {
        return cache[n];
    }

    /* compute new terms up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 9) continue;  /* base values already set */

        seq_t t1 = 2 * cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = 3 * cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = 3 * cache[i - 6];
        seq_t t7 = cache[i - 7];
        seq_t t8 = 2 * cache[i - 8];
        seq_t t9 = cache[i - 9];

        cache[i] = t1 + t2 - t3 - t4 + t5 + t6 - t7 - t8 + t9;
    }

    return cache[n];
}
