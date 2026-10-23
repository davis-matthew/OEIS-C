/*
 * A038889  Primes p such that 17 is a square mod p.
 * https://oeis.org/A038889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 17 is a quadratic residue modulo p.
   Returns the n‑th such prime (1‑based index). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;   /* number of terms stored (>=0) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Keep track of the next integer to test for primality */
    static uint64_t next_test = 2;

    /* Grow the cache as needed */
    if (n > cache_cnt) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 32;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = /* cast needed for C++ */ (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Find new terms until we reach the requested index */
        while (cache_cnt < (size_t)n) {
            /* Find next prime >= next_test */
            for (uint64_t p = next_test;; ++p) {
                if (is_prime((idx_t)p)) {
                    /* Test Jacobian: 17 is a square mod p (or p==2,17) */
                    if (p == 2 || p == 17 || jacobi_symbol(17ULL, p) == 1) {
                        cache[cache_cnt++] = (seq_t)p;
                    }
                    next_test = p + 1;
                    break;            /* go to next term if needed */
                }
            }
        }
    }

    return cache[(size_t)n - 1];
}
