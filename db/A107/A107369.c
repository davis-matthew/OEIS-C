/*
 * A107369  Numbers n such that 103*n + 101 is prime.
 * https://oeis.org/A107369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 103*n + 101 is prime
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* Cache of found terms – the list is built lazily */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of terms stored */
    static idx_t cap   = 0;   /* allocated capacity */
    static uint64_t next_n = 0;   /* next n to test for primality */

    /* Grow the cache until it contains at least n entries */
    while (size < n)
    {
        /* candidate n to test */
        uint64_t cand = next_n;

        /* compute 103*n+101 without overflow */
        unsigned __int128 prod = (unsigned __int128)103 * cand + 101;
        if (prod > (unsigned __int128)UINT64_MAX) {
            /* too large for the provided prime test – stop */
            abort();
        }

        uint64_t p = (uint64_t)prod;

        /* test primality */
        if (is_prime(p)) {
            /* ensure capacity */
            if (size == cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = (seq_t)cand;
        }

        ++next_n;
    }

    /* cache is 0‑based, n is 1‑based */
    return cache[n - 1];
}
