/*
 * A097959  Primes p such that p divides 6^((p-1)/2) - 5^((p-1)/2).
 * https://oeis.org/A097959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9705 (2026-11-03)
 * Helpers:   is_prime, mem
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
                seq_t *tmp = mem_resize(seq_t, buf, new_cap);
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
