/*
 * A141888  Primes congruent to 2 mod 21.
 * https://oeis.org/A141888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 2 (mod 21)
 * Range:     u8 n <= 6, u16 n <= 553, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of computed primes satisfying p ≡ 2 (mod 21) */
    static seq_t *primes = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t count = 0;   /* number of primes stored */
    static idx_t   last_k = 0; /* last k used in candidate 21*k+2 */

    /* Initialise cache with the first prime 2 */
    if (count == 0) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!primes) return 0;      /* failure, shouldn't happen */
        primes[0] = (seq_t)2;        /* 2 ≡ 2 (mod 21) */
        count = 1;
    }

    /* Extend the cache until it contains at least n primes */
    while (count < n) {
        ++last_k;                                   /* next k */
        seq_t cand = (seq_t)21 * (seq_t)last_k + (seq_t)2;
        if (is_prime((idx_t)cand)) {                /* candidate is prime */
            if (count >= cap) {                    /* grow the array */
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) return 0;                /* allocation failure */
                primes = tmp;
            }
            primes[count++] = cand;
        }
    }

    return primes[n - 1];
}
