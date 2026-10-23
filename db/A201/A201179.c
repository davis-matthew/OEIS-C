/*
 * A201179  Primes of the form 6n^3+1.
 * https://oeis.org/A201179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 6n^3+1 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    /* Static cache for found primes and the next candidate index */
    static seq_t *primes = NULL;            /* array of primes found so far */
    static size_t primes_capacity = 0;      /* allocated capacity of primes[] */
    static idx_t  current_k = 1;            /* next k to test for primality */
    static idx_t  primes_found = 0;         /* number of primes stored */

    if (n == 0) return (seq_t)0;            /* undefined below the sequence offset */

    /* Return cached value if already computed */
    if (n <= primes_found) {
        return primes[n - 1];
    }

    /* Grow the primes array as necessary */
    if (primes_capacity == 0) {
        primes_capacity = 16;
        primes = (seq_t *)malloc(primes_capacity * sizeof(seq_t));
        if (!primes) exit(1);
    }

    /* Search further until we have at least n primes */
    while (primes_found < n) {
        /* Compute p = 6 * current_k^3 + 1 using the target integer type */
        seq_t k = (seq_t)current_k;
        seq_t p = ((seq_t)6 * k * k * k) + 1;

        if (is_prime_seq(p)) {
            /* Store the new prime, extending the array if needed */
            if (primes_found == primes_capacity) {
                size_t new_cap = primes_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                primes = tmp;
                primes_capacity = new_cap;
            }
            primes[primes_found++] = p;
        }
        ++current_k;
    }

    return primes[n - 1];
}
