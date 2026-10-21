/*
 * A097959  Primes p such that p divides 6^((p-1)/2) - 5^((p-1)/2).
 * https://oeis.org/A097959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097959: primes p such that p divides 6^((p‑1)/2) – 5^((p‑1)/2)
   Equivalent to the Jacobi/Kronecker symbol (30|p) being +1.
   The function returns the n‑th prime satisfying this condition,
   computed on demand using a growing static buffer. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Provided by the build system. */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/jacobi_symbol.h"     /* int jacobi_symbol(uint64_t a, uint64_t n); */

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* 0‑based buffer: buf[i] holds the (i+1)‑th term. */
    static seq_t *buf   = NULL;  /* dynamic array of found primes   */
    static size_t  len  = 0;     /* number of stored terms          */
    static size_t  cap  = 0;     /* capacity of the buffer          */
    static idx_t   cand = 2;     /* next integer candidate to test  */

    while (len < n) {
        /* Find the next prime candidate. */
        while (!is_prime(cand))
            cand++;

        /* Check the Jacobi symbol (30|p). */
        if (jacobi_symbol(30, (uint64_t)cand) == 1) {
            /* Grow buffer if necessary. */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(*buf));
                if (!tmp) abort();        /* allocation failure */
                buf = tmp;
                cap = new_cap;
            }
            buf[len++] = (seq_t)cand;   /* store the prime */
        }
        cand++;   /* move to the next integer for the following loop */
    }

    return buf[n - 1];
}
