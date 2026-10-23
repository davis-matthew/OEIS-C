/*
 * A182784  Primes of the form 2*n^4-1.
 * https://oeis.org/A182784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A182783(n)^4-1 (Primes of the form 2*n^4‑1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cache of previously found primes in the sequence */
    static seq_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static idx_t next_k = 2;   /* smallest k to test next */

    if (n == 0) return 0;     /* not in OEIS range; safe guard */

    /* If we already have the requested prime, return it */
    if (primes_len >= (size_t)n) {
        return primes[n - 1];
    }

    /* One‑time allocation of the cache array */
    if (!primes) {
        primes_cap = 128;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) return (seq_t)0;  /* allocation failure */
    }

    /* Generate primes of the form 2*k^4-1, starting from next_k */
    for (idx_t k = next_k; ; ++k) {
        seq_t k_big = (seq_t)k;
        seq_t k2    = k_big * k_big;   /* k^2 */
        seq_t k4    = k2 * k2;         /* k^4 */
        seq_t val   = k4 * 2 - 1;      /* 2*k^4-1 */

        if (isPrimeGeneric(val)) {
            /* Append to cache */
            if (primes_len >= primes_cap) {
                primes_cap <<= 1;
                primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                if (!primes) return (seq_t)0;  /* allocation failure */
            }
            primes[primes_len++] = val;
            next_k = k + 1;           /* Advance to next k for future calls */

            if (primes_len >= (size_t)n) {
                return val;           /* nth prime found */
            }
        }
    }
}
