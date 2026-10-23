/*
 * A166547  Primes of the form 100*k+7.
 * https://oeis.org/A166547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Sequence: nth prime of the form 100*k+7 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current capacity of the cache      */
    static size_t len   = 0;   /* number of primes stored            */
    static idx_t  k      = 0;   /* current k to test (candidate = 100*k+7) */

    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate primes until we have n of them */
    while (len < n) {
        seq_t cand = (seq_t)100 * (seq_t)k + (seq_t)7;  /* 100*k + 7 */
        if (isPrimeGeneric(cand)) {
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = cand;
        }
        ++k;
    }

    return cache[(size_t)(n - 1)];
}
