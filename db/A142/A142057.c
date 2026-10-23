/*
 * A142057  Primes congruent to 14 mod 33.
 * https://oeis.org/A142057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  primes congruent to 14 modulo 33
   a(n) = the n‑th prime p with p ≡ 14 (mod 33),  a(1) = 47
   The function builds the primes on demand, caching the results.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime test for seq_t values */
#include "../utils/isPrimeGeneric.h"

/* Return the n‑th term of A142057 for any seq_t type */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a harmless sentinel */
    if (n == 0)
        return 0;

    /* Static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;

    /* The next candidate to test, starting at the first number ≡14 (mod 33) */
    static seq_t next = 14;

    /* Keep generating candidates until we have the requested index */
    while (len < n) {
        if (isPrimeGeneric(next)) {
            /* Expand cache if necessary */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – give up */
                    return 0;
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = next;
        }
        /* Move to the next number ≡14 (mod 33) */
        next += 33;
    }

    return cache[n - 1];
}
