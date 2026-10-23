/*
 * A142784  Primes congruent to 57 mod 59.
 * https://oeis.org/A142784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 57 modulo 59 (OEIS A142784)
 * Range:     u8 none, u16 n <= 106, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates primes of that form, caching results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *primes = NULL;      /* array of primes */
    static size_t  capacity = 0;      /* allocated capacity of primes array */
    static size_t  count    = 0;      /* number of primes stored */

    /* Current k such that candidate = 59*k + 57 */
    /* For the first prime (293) we have k = 4 : 59*4 + 57 = 293 */
    static idx_t current_k = 4;

    /* Return immediately if n is zero (offset not used) */
    if (n == 0) return 0;

    /* Generate primes until we have at least n of them */
    while (count < (size_t)n) {
        idx_t cand = 59 * current_k + 57;  /* candidate number in the residue class */

        /* Test candidate for primality */
        if (is_prime_big((seq_t)cand)) {
            /* Ensure array has enough space, then store the prime */
            grow_seq(&primes, &capacity, count + 1);
            primes[count++] = (seq_t)cand;
        }

        /* Move to next candidate */
        current_k++;
    }

    /* Return the nth prime (1‑based indexing) */
    return primes[n - 1];
}
