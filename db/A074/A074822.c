/*
 * A074822  Primes p such that p + 4 is prime and p == 9 (mod 10).
 * https://oeis.org/A074822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 9 (mod 10) and p + 4 is
 *            prime. The function searches numbers of the form 10 k – 1 (k ≥
 *            2) for primality using the supplied is_prime(idx_t) routine,
 *            collects qualifying primes in a static cache, and returns the
 *            desired term
 * Range:     u8 n <= 4, u16 n <= 271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n) {
    /* static cache of collected terms */
    static seq_t *terms = NULL; /* array of terms */
    static size_t   len  = 0;   /* number of terms cached          */
    static size_t   cap  = 0;   /* capacity of cache                */
    static uint64_t next_k = 2; /* next k to try (p = 10 k – 1)    */

    /* generate terms up to requested index if needed */
    if (n > len) {
        /* increase capacity if necessary */
        if (cap < n) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out of memory */
            terms = tmp;
            cap   = new_cap;
        }

        /* test successive candidates of the form 10 k – 1 */
        while (len < n) {
            uint64_t p64 = 10 * next_k - 1;          /* last digit 9 */
            idx_t    p  = (idx_t)p64;                /* cast to the type used by is_prime() */
            if (is_prime(p) && is_prime(p + 4)) {
                /* store the prime; implicit conversion to seq_t */
                terms[len++] = (seq_t)p;
            }
            ++next_k;
        }
    }

    return terms[n - 1];     /* n is 1‑based */
}
