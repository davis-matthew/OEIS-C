/*
 * A157253  Arises in combinatorial approach to the power of 2 in the number
 *          of involutions.
 * https://oeis.org/A157253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=2; a(2*n) = a(2*n-2) +
 *            (n-1)*a(2*n-3) + 2*C(n-1,2)*a(2*n-4) + 3*C(n-1,3)*a(2*n-8)
 *            a(2*n+1) = a(2*n) + n*a(2*n-1)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 24, u64 n <= 39, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache  = NULL;   /* array of a(i) values          */
    static idx_t  cap    = 0;      /* current allocated size (elements) */
    static idx_t  maxcalc = 0;     /* highest index that has a defined value */

    /* --- Allocate initial cache and set base terms -------------- */
    if (!cache) {
        cap = 4;                                      /* to hold indices 0..3 */
        cache = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        maxcalc = 3;
    }

    /* --- Ensure the cache can hold a(n) ------------------------ */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        /* grow to next power of two for amortized O(1) growth */
        while (new_cap <= cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache, sizeof(seq_t) * (size_t)new_cap);
        if (!tmp) abort();
        cache = tmp;
        /* initialise the newly allocated slots to zero */
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* --- Compute terms that have not yet been calculated -------- */
    for (idx_t i = maxcalc + 1; i <= n; ++i) {
        if ((i & 1) == 0) {                /* even index: a(2*n)  */
            idx_t k = i >> 1;              /* n in the recurrence   */
            seq_t val = cache[i - 2];      /* a(2*n-2)               */

            if (k > 1)                     /* (n-1)*a(2*n-3)         */
                val += (seq_t)(k - 1) * cache[i - 3];

            if (k > 2) {                   /* 2*C(n-1,2)*a(2*n-4)    */
                seq_t b21 = binomial((idx_t)(k - 1), (idx_t)2);
                val += (seq_t)2 * b21 * cache[i - 4];
            }

            if (k > 3) {                   /* 3*C(n-1,3)*a(2*n-8)    */
                seq_t b31 = binomial((idx_t)(k - 1), (idx_t)3);
                val += (seq_t)3 * b31 * cache[i - 8];
            }

            cache[i] = val;
        } else {                          /* odd index: a(2*n+1)    */
            idx_t k = (i - 1) >> 1;        /* n in the recurrence   */
            cache[i] = cache[i - 1] + (seq_t)k * cache[i - 2];
        }
    }
    maxcalc = (maxcalc > n) ? maxcalc : n;

    return cache[n];
}
