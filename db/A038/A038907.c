/*
 * A038907  Primes p such that 33 is a square mod p.
 * https://oeis.org/A038907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 897 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime p for which 33 is a quadratic residue modulo p
   (i.e. the Jacobi symbol (33/p) = 1 or the divisor case p = 3 or 11). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Memoisation structures */
    static seq_t *cache = NULL;          /* array of qualifying primes */
    static size_t cache_size = 0;        /* number of stored primes */
    static size_t cache_cap  = 0;        /* allocated capacity of the array */

    static uint64_t cand = 2;            /* next candidate prime to test */

    /* Generate primes until we have at least n values */
    while (cache_size < (size_t)n) {
        if (is_prime(cand)) {
            /* 33 is a square modulo p if (33/p)=1, or p divides 33
               (the only such primes are 3 and 11). */
            if (cand == 2 || cand == 3 || cand == 11 ||
                jacobi_symbol(33, cand) == 1) {
                /* Grow cache if needed */
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) {
                        /* Allocation failure – abort */
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)cand;
            }
        }

        /* Advance candidate */
        if (cand == 2)
            cand = 3;
        else
            cand += 2;
    }

    return cache[n - 1];
}
