/*
 * A142756  Primes congruent to 29 mod 59.
 * https://oeis.org/A142756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 123, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 29 mod 59 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_count = 0;
    static size_t primes_cap = 0;

    if (n > primes_count) {
        uint64_t candidate = 29;
        if (primes_count > 0) {
            /* The last stored prime is ≡ 29 (mod 59),
               jump to the next candidate in the arithmetic progression. */
            uint64_t last = (uint64_t)primes[primes_count - 1];
            candidate = last + 59;
        }

        /* Ensure capacity of the cache. */
        if (primes == NULL) {
            primes_cap = (size_t)n + 10;
            primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        } else if (primes_count + (size_t)(n - primes_count) > primes_cap) {
            size_t new_cap = primes_cap * 2 + 10;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Generate and cache more primes until we have at least n of them. */
        while (primes_count < (size_t)n) {
            if (is_prime_big((seq_t)candidate))
                primes[primes_count++] = (seq_t)candidate;
            candidate += 59;
        }
    }

    return primes[n - 1];
}
