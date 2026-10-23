/*
 * A188382  Primes of the form 8*n^2 + 2*n + 1.
 * https://oeis.org/A188382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 8*m^2 + 2*m + 1, where m ≥ 1
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 36, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Compute the nth term of A188382 for n ≥ 1. */
seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_m = 1;   /* m to test next */

    if (n == 0) {
        /* undefined below the offset; return 0 for safety */
        return (seq_t)0;
    }

    /* Extend cache until it contains at least n primes */
    while (primes_len < n) {
        /* compute candidate p = 8*m^2 + 2*m + 1 using 64‑bit arithmetic */
        uint64_t m64 = (uint64_t)next_m;
        uint64_t p64 = m64 * m64 * 8ULL + m64 * 2ULL + 1ULL;

        /* cast to seq_t (safe for values that fit; otherwise wrap) */
        seq_t p = (seq_t)p64;

        if (is_prime_big(p)) {
            /* grow cache if needed */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory – return 0 as a graceful failure */
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = p;
        }
        ++next_m;
    }

    /* return the required term (1‑based indexing) */
    return primes[n - 1];
}
