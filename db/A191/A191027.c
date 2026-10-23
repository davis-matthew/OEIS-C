/*
 * A191027  Primes that are nonzero squares mod 37.
 * https://oeis.org/A191027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p with Legendre symbol (p/37) = +1.
   a(n) is the nth prime p such that (p/37) = +1, with offset 1. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour below offset is irrelevant */

    /* Static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t candidate = 2;   /* Next integer to test */

    if (len < n) {
        /* Grow cache if necessary */
        if (cap < n) {
            size_t new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failure sentinel */
            cache = tmp;
            cap = new_cap;
        }

        while (len < n) {
            /* Skip even numbers greater than 2 */
            if (candidate > 2 && (candidate & 1) == 0) {
                candidate++;
                continue;
            }

            if (!is_prime(candidate)) {
                candidate++;
                continue;
            }

            /* Legendre symbol (candidate/37) == +1? */
            if (jacobi_symbol(candidate, 37) == 1) {
                cache[len++] = (seq_t)candidate;
            }
            candidate++;
        }
    }

    return cache[n - 1];
}
