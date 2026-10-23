/*
 * A142817  Primes congruent to 19 mod 61.
 * https://oeis.org/A142817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 19 mod 61 (OEIS A142817)
   Computation: successive numbers of the form 19 + 61*k are tested for
   primality via is_prime(). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, return 0 for safety */

    /* static cache for found primes */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    static idx_t next_k = 0; /* next k to test, so candidate = 19 + 61*k */

    /* ensure cache capacity */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
    }

    /* generate primes until we have n of them */
    while (primes_cnt < n) {
        seq_t candidate = (seq_t)19 + (seq_t)61 * (seq_t)next_k;
        if (is_prime((idx_t)candidate)) {
            /* grow cache if needed */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* allocation failed; return 0 or handle error */
                    return 0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }
        next_k++;
    }

    return primes[n - 1];
}
