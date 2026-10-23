/*
 * A122262  Number of numbers having only factors that are Pierpont primes.
 * https://oeis.org/A122262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k ≤ n : every prime factor of k is a Pierpont prime}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_pierpont_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_pierpont_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* undefined for n < 1, return 0 */
    if (n < 1U) return 0;

    /* Convert to a type that can hold the array size */
    size_t limit = (size_t)n;

    /* Allocate arrays */
    uint8_t *good    = (uint8_t *)malloc(limit + 1);
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!good || !is_prime) {
        free(good);
        free(is_prime);
        return 0;
    }

    /* Initialize: all numbers are good initially, 0 and 1 are not considered prime */
    good[0] = 0;
    good[1] = 1;          /* 1 is counted */
    for (size_t i = 2; i <= limit; ++i) { good[i] = 1; is_prime[i] = 1; }
    is_prime[0] = is_prime[1] = 0;

    /* Classic sieve of Eratosthenes to mark non‑primes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* For each prime that is NOT Pierpont, mark all its multiples as bad */
    for (size_t p = 2; p <= limit; ++p) {
        if (is_prime[p] && !is_pierpont_prime(p)) {
            for (size_t m = p; m <= limit; m += p) good[m] = 0;
        }
    }

    /* Count good numbers up to limit */
    size_t count = 1;               /* include 1 */
    for (size_t i = 2; i <= limit; ++i) if (good[i]) ++count;

    /* Clean up */
    free(good);
    free(is_prime);

    /* Return as seq_t */
    return (seq_t)count;
}
