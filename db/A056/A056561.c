/*
 * A056561  Numbers n such that n^2 + n + 41 (known as Euler's polynomial) is
 *          prime.
 * https://oeis.org/A056561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth non‑negative integer m such that m^2 + m + 41 is
 *            prime. The sequence starts as
 *            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
 *            i.e. indices at which Euler's polynomial yields a prime
 * Range:     u8 n <= 15, u16 n <= 530, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache of computed sequence values.  */
    static seq_t *cache = NULL;      /* array of sequence values */
    static size_t  cache_cap = 0;    /* allocated capacity */
    static idx_t   filled = 0;       /* number of terms currently stored */
    static idx_t   next_m = 0;       /* next integer to examine */

    /* Ensure some capacity, growing by a factor of two when needed. */
    if (cache_cap == 0) {
        cache_cap = 16;                       /* initial arbitrary size */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (filled < n) {
        /* Compute m^2 + m + 41 in seq_t arithmetic. */
        seq_t m    = (seq_t)next_m;
        seq_t poly = m * m + m + (seq_t)41;

        if (isPrimeGeneric(poly)) {
            /* Store the new term.  Grow the cache if full. */
            if (filled == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[filled++] = m;
        }
        next_m++;
    }

    /* Return the desired term (indices in the function are 1‑based). */
    return cache[n - 1];
}
