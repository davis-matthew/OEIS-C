/*
 * A142414  Primes congruent to 1 mod 49.
 * https://oeis.org/A142414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 148, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 1 (mod 49).
   a(n) is the n‑th prime p such that p ≡ 1 (mod 49), 1‑based. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n‑th prime ≡ 1 (mod 49), 1‑based index. */
seq_t a(idx_t n)
{
    /* Cached list of found primes. */
    static seq_t *primes = NULL;
    static size_t count = 0;   /* number of primes stored */
    static size_t cap   = 0;   /* current allocation capacity */
    static uint64_t k    = 0;   /* multiplier for candidates 49*k+1 */

    /* Undefined behaviour for n == 0; just return 0. */
    if (n == 0) return 0;

    /* Allocate cache on first call. */
    if (!primes) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!primes) return 0;   /* allocation failed */
    }

    /* Generate candidates until we have at least n primes. */
    while (count < n) {
        ++k;
        uint64_t cand64 = 49ULL * k + 1ULL;   /* candidate in a type that won't overflow */

        seq_t cand = (seq_t)cand64;           /* cast to seq_t; safe for values that fit */

        if (is_prime_big(cand)) {
            if (count == cap) {               /* grow the cache if needed */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) return primes[count - 1]; /* out of memory; return last prime */
                primes = tmp;
            }
            primes[count++] = cand;
        }
    }

    return primes[n - 1];
}
