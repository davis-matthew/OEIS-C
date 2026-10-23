/*
 * A049880  a(n) is the number of distinct sums of 2 different primes chosen
 *          from the first n primes.
 * https://oeis.org/A049880
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums of two different primes among the
 *            first n primes
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0; /* offset starts at 2 */
    }

    uint32_t *primes = NULL;
    size_t prime_count = 0;
    sieve_first_n_primes(n, &primes, &prime_count);

    /* Highest possible sum is largest + second largest prime */
    uint32_t max_sum = primes[prime_count - 1] + primes[prime_count - 2];
    size_t bitmap_size = (size_t)max_sum + 1;

    uint8_t *seen = (uint8_t*)calloc(bitmap_size, sizeof(uint8_t));
    seq_t unique = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t pi = primes[i];
        for (size_t j = i + 1; j < prime_count; ++j) {
            uint32_t s = pi + primes[j];
            if (!seen[s]) {
                seen[s] = 1;
                ++unique;
            }
        }
    }

    free(primes);
    free(seen);

    return unique;
}
