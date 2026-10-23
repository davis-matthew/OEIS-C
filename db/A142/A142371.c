/*
 * A142371  Primes congruent to 20 mod 47.
 * https://oeis.org/A142371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 136, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142371: primes ≡ 20 (mod 47)
   Computation: iterate over numbers ≡20 (mod 47) and test
   each for primality, storing the primes found.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state – cumulative cache of found primes */
    static seq_t *primes = NULL;   /* array of primes found   */
    static size_t len   = 0;        /* number of primes stored */
    static size_t cap   = 0;        /* allocated capacity      */
    static seq_t current = 20;      /* last tested candidate   */

    /* generate primes until we have at least n of them */
    while (len < n) {
        /* advance to the next candidate ≡20(mod 47) */
        seq_t candidate = (seq_t)((seq_t)current + (seq_t)47);
        if (candidate < current)            /* overflow – stop */
            break;
        current = candidate;

        /* skip non‑primes */
        while (!is_prime_big(candidate)) {
            candidate = (seq_t)((seq_t)candidate + (seq_t)47);
            if (candidate < current)       /* overflow – stop */
                break;
            current = candidate;
        }
        if (candidate < current)            /* overflow – stop */
            break;

        /* store the newly found prime */
        if (len == cap) {
            size_t old_cap = cap;
            cap = cap ? cap << 1 : 64;
            primes = seq_realloc(primes, old_cap, cap);
        }
        primes[len++] = candidate;
    }

    return primes[n - 1];
}
