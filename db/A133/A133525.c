/*
 * A133525  Sum of third powers of four consecutive primes.
 * https://oeis.org/A133525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..3} p_{n+k}^3, where p_i is the i‑th prime. Sieve
 *            of Eratosthenes is used to generate primes
 * Range:     u8 none, u16 n <= 7, u32 n <= 170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of primes (0‑based indices) */
static uint64_t *prime_list = NULL; /* array of primes */
static size_t     prime_cnt  = 0;    /* number of primes stored */

/* Ensure that at least (required_n+3) primes are available in prime_list. */
static void generate_primes(idx_t required_n)
{
    size_t needed_primes = (size_t)required_n + 3;
    if (prime_cnt >= needed_primes)
        return; /* already enough */

    /* Start with an initial sieve limit; double until enough primes. */
    size_t limit = 200000; /* sufficient for n ≈ 10 000 */
    while (true) {
        /* Allocate mask: 0 = prime, 1 = composite */
        uint8_t *mask = (uint8_t *)calloc(limit + 1, 1);
        if (!mask)
            return; /* allocation failed – give up, result will be 0 */

        for (size_t i = 2; i * i <= limit; ++i) {
            if (!mask[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    mask[j] = 1;
            }
        }

        /* Count primes up to 'limit' */
        size_t count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!mask[i]) ++count;

        if (count >= needed_primes) {
            /* Build array of primes */
            uint64_t *list = (uint64_t *)malloc(count * sizeof(uint64_t));
            if (!list) {
                free(mask);
                return; /* allocation failed */
            }
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (!mask[i])
                    list[idx++] = (uint64_t)i;

            /* Replace previous cache */
            free(prime_list);
            prime_list = list;
            prime_cnt  = count;
            free(mask);
            return;
        }

        /* Not enough primes – enlarge the sieve limit */
        free(mask);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */

    /* Ensure that at least n+3 primes are available. */
    generate_primes(n);

    if (prime_cnt < (size_t)n + 3)
        return (seq_t)0; /* still insufficient */

    /* Compute the sum of cubes of four consecutive primes starting at index n-1. */
    idx_t idx = n - 1;                     /* 0‑based index */
    seq_t sum = 0;
    for (size_t k = 0; k < 4; ++k) {
        seq_t p = (seq_t)prime_list[(size_t)idx + k];
        sum += p * p * p;
    }
    return sum;
}
