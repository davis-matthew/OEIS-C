/*
 * A108567  a(0) = 0, a(1) = a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 8, a(6) =
 *          16, for n>5: a(n+1) = SORT[ a(n) + a(n-1) + a(n-2) + a(n-3) +
 *          a(n-4) + a(n-5) + a(n-6)], where SORT places digits in ascending
 *          order and deletes 0's.
 * https://oeis.org/A108567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=8, a(6)=16; for n
 *            ≥ 7: a(n) = SORT[
 *            a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7) ] where SORT
 *            puts the decimal digits of its argument in ascending order and
 *            removes all zeros
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sort_and_remove_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sort_and_remove_zeros.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t  cap       = 0;       /* allocated capacity */
    static idx_t   max_done  = (idx_t)-1;   /* highest n computed; -1 means uninitialized */

    /* initialize cache with the first seven terms */
    if (cache == NULL) {
        const seq_t init_vals[7] = {0, 1, 1, 2, 4, 8, 16};
        cap = 16;                     /* start with room for 16 terms */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* fatal error if memory cannot be allocated */

        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        max_done = 6;
    }

    /* if term already computed, return it */
    if (n <= max_done)
        return cache[n];

    /* compute successive terms until n is reached */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        /* ensure there is space for cache[i] */
        if ((size_t)i >= cap) {
            size_t newcap = cap * 2;
            if ((size_t)(i + 1) > newcap)
                newcap = i + 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = newcap;
        }

        /* sum the previous seven terms */
        seq_t sum = 0;
        for (int k = 0; k < 7; ++k)
            sum += cache[i - 1 - k];

        cache[i] = sort_and_remove_zeros(sum);
    }

    max_done = n;
    return cache[n];
}
