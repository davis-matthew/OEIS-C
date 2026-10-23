/*
 * A142685  Primes congruent to 32 mod 57.
 * https://oeis.org/A142685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th prime congruent to 32 mod 57 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *primes = NULL;
    static size_t count   = 0;
    static size_t cap     = 0;
    static idx_t  last_k  = 0;   /* last tested residue index (k in 57k+32) */

    if (n == 0) return 0;  /* behavior below offset is irrelevant */

    /* Return cached value if already computed */
    if (count >= n)
        return primes[n - 1];

    /* Grow the prime table as needed */
    if (primes == NULL) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!primes) return 0; /* allocation failure; unlikely for required sizes */
    }

    /* Generate primes until we reach the desired index n */
    while (count < n) {
        ++last_k;
        idx_t cand = 57 * last_k + 32;   /* candidate in the form 57k+32 */

        if (is_prime(cand)) {
            if (count == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                primes = tmp;
            }
            primes[count++] = (seq_t)cand;
        }
    }

    return primes[n - 1];
}
