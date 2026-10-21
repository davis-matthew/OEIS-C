/*
 * A092402  Primes of the form p+8 where p is a prime.
 * https://oeis.org/A092402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 893, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a sequence where a(n) is the n‑th prime q such that q‑8 is also prime. */
/* The implementation uses a simple sieve and rescales the bound until the */
/* required term is found. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 : a(1) is the first term */
    uint64_t limit = n * 20;               /* initial rough bound */
    if (limit < 100) limit = 100;

    for (;; limit *= 2) {                  /* increase until we find n terms */
        bool *isPrime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!isPrime) return 0;            /* allocation failure – should not happen */

        /* Sieve of Eratosthenes */
        for (uint64_t i = 0; i <= limit; ++i) isPrime[i] = true;
        isPrime[0] = isPrime[1] = false;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (isPrime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    isPrime[j] = false;
            }
        }

        /* Count qualifying primes */
        size_t count = 0;
        seq_t result = 0;
        for (uint64_t q = 11; q <= limit; q += 2) {   /* start from 11, smallest term */
            if (isPrime[q] && q >= 9 && isPrime[q - 8]) {
                ++count;
                if (count == n) {
                    result = (seq_t)q;
                    free(isPrime);
                    return result;
                }
            }
        }
        free(isPrime);                      /* not enough terms – double the bound */
    }
}
