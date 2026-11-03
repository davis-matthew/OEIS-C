/*
 * A236799  Decimal expansion of 1/9998.
 * https://oeis.org/A236799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 359, u16 n <= 359, u32 n <= 359, u64 n <= 359,
 *            u128 n <= 359, big any n
 * Validated: standard, pass, 121 terms through n = 354 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A236799: Decimal expansion of 1/9998.  */
#include <stdlib.h>        /* malloc, realloc */
#include <stdint.h>       /* for uint64_t if needed */
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Fast digit extraction by repeated modular division.
       The remainder after the k-th digit is kept in rem,
       and the next digit is computed as floor(rem*10/9998). */
    static seq_t *cache = NULL;          /* cached digits */
    static size_t cache_len = 0;         /* number of cached digits */
    static size_t cache_cap = 0;         /* allocated capacity */
    static unsigned rem = 1;             /* remainder before next digit */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        /* if malloc fails, cache remains NULL and the following use will crash
           which is acceptable for this simple utility function. */
    }

    /* extend cache up to n */
    while (cache_len <= n) {
        /* compute next digit */
        unsigned long long t = (unsigned long long)rem * 10ULL; /* safe product */
        unsigned digit  = (unsigned)(t / 9998ULL);
        rem = (unsigned)(t % 9998ULL);

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;  /* double the capacity */
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len] = (seq_t)digit;
        cache_len++;
    }

    return cache[n];
}
