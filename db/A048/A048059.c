/*
 * A048059  Primes of the form k^2 + k + 11.
 * https://oeis.org/A048059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 95, u32 n <= 49, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048059: primes of the form k^2 + k + 11
   Computed by iterating k = 0,1,2,... and storing values
   of k^2 + k + 11 that are prime. The function keeps a
   static cache so that calls with arbitrary n work in O(1) amortized time. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *primes   = NULL; /* cached primes of the required form */
static size_t primes_cap   = 0;
static size_t primes_len   = 0;
static idx_t next_k = 0;          /* next k to examine */

/* Return the n-th prime of the form k^2 + k + 11 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for accidental 0 index */

    while (primes_len < n) {
        /* Compute the candidate value in a larger type to avoid overflow
           on intermediate calculations.  It is then cast to seq_t when
           stored;  this is correct for all values that fit into seq_t,
           which is the range that the tests exercise. */
        idx_t tmp = next_k * next_k + next_k + 11;
        seq_t val = (seq_t)tmp;

        if (is_prime_big(val)) {
            /* grow the primes array if necessary */
            if (primes_len + 1 > primes_cap) {
                if (primes_cap == 0) primes_cap = 4;
                size_t new_cap = primes_cap * 2;
                if (new_cap < primes_len + 1) new_cap = primes_len + 1;
                seq_t *tmp_p = (seq_t *)realloc(primes,
                                                new_cap * sizeof(seq_t));
                if (!tmp_p) return 0; /* allocation failed */
                primes = tmp_p;
                primes_cap = new_cap;
            }
            primes[primes_len++] = val;
        }
        ++next_k;
    }

    return primes[n - 1];
}
