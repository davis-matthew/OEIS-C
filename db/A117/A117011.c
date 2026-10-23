/*
 * A117011  Decimal value of binary number whose n-th 1 is followed by n^3
 *          0's.
 * https://oeis.org/A117011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=0, a(1)=5; for n≥1: a(n+1) = 1 + a(n) * 2^(1 + (n+1)^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   cached = 0;   /* number of terms stored in cache */

    /* initialise cache if it does not exist yet */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cached   = 2;                               /* a(0) and a(1) stored */
    }

    /* if requested index already cached, return it */
    if (n < cached)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = cached; i <= n; ++i) {
        /* recurrence is valid for i ≥ 2; a(0) and a(1) are pre‑filled */
        idx_t shift = i * i * i + 1;              /* (i)^3 + 1 for new term */
        seq_t pow2_shift = pow2(shift);           /* 2^(shift) as seq_t */
        cache[i] = cache[i-1] * pow2_shift + (seq_t)1;
    }

    cached = n + 1;
    return cache[n];
}
