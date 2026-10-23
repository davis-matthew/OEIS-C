/*
 * A113530  Semiprimes in A003215.
 * https://oeis.org/A113530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intersection of A003215 and A001358:
   a(n) is the nth value of 3k²+3k+1 that is a semiprime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_semiprime_uint64.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;   /* array of a(k) values */
    static size_t   cap   = 0;    /* allocated capacity of cache */
    static size_t   sz    = 0;    /* number of terms stored */
    static uint64_t next_k = 0;   /* next k to test in A003215 */

    if (n == 0) return 0; /* offset is 1; a(0) is undefined */

    /* Ensure the cache can hold at least n elements. */
    if (sz < (size_t)n) {
        if (cap < (size_t)n) {
            size_t newcap = cap ? cap * 2 : 8;
            if (newcap < (size_t)n) newcap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }

        /* Find further semiprime values of 3*k^2+3*k+1. */
        while (sz < (size_t)n) {
            uint64_t k     = next_k;
            uint64_t val64 = 3ULL * k * k + 3ULL * k + 1ULL;
            if (is_semiprime_uint64(val64)) {
                cache[sz++] = (seq_t)val64;
            }
            ++next_k;
        }
    }
    return cache[(size_t)n - 1];
}
