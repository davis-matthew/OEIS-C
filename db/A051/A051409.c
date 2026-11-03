/*
 * A051409  a(n+1) = a(n) + sum of digits of (a(n)^3).
 * https://oeis.org/A051409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + sum_of_digits( a(n-1)^3 ), a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 957 (2026-11-01)
 * Helpers:   digit_sum, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* undefined below offset */

    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cap  = 0;

    /* initialise the cache with the first term */
    if (cap == 0) {
        cache = seq_realloc(NULL, 0, 1);          /* allocate one element */
        cap   = 1;
        cache[0] = (seq_t)4;                     /* a(1) */
    }

    /* grow and fill the cache if the requested index is beyond the current size */
    if ((size_t)n > cap) {
        seq_t *tmp = seq_realloc(cache, cap, (size_t)n);
        cache = tmp;
        for (size_t i = cap; i < (size_t)n; ++i) {
            seq_t prev = cache[i-1];                 /* a(i) */
            seq_t cube = ipow(prev, (seq_t)3);     /* prev^3 */
            seq_t add  = digit_sum(cube);            /* sum of digits */
            cache[i] = prev + add;                   /* a(i+1) */
        }
        cap = (size_t)n;
    }

    return cache[n-1];                              /* return a(n) */
}
