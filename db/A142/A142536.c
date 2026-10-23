/*
 * A142536  Primes congruent to 6 mod 53.
 * https://oeis.org/A142536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 6 modulo 53
 * Range:     u8 n <= 1, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that p ≡ 6 (mod 53), with offset 1 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored */
    static idx_t next_candidate = 59; /* first prime ≡ 6 mod 53 */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    while (cache_size < n) {
        if (is_prime(next_candidate)) {
            /* grow cache by one element */
            seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – terminate gracefully */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            /* cast from idx_t (uint64_t) to seq_t (may be unsigned or arbitrary‑precise) */
            cache[cache_size] = (seq_t)next_candidate;
            ++cache_size;
        }
        next_candidate += 53;
    }

    return cache[n - 1];
}
