/*
 * A074953  Numbers equidistant from consecutive twin prime pairs.
 * https://oeis.org/A074953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A006512(n+1) + A001359(n)) / 2
 * Range:     u8 n <= 10, u16 n <= 831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static bool initialized = false;
    static idx_t *low  = NULL;   /* A006512: lower prime of each twin pair */
    static idx_t *high = NULL;   /* A001359: higher prime  of each twin pair */
    static size_t twin_count = 0; /* number of twin prime pairs found */

    if (!initialized) {
        /* Eratosthenes sieve up to a fixed bound that guarantees enough
           twin primes for n up to about 10000.
           2,000,000 produces more than 12,500 twin pairs (an ample margin). */
        const size_t sieve_lim = 2000000;

        unsigned char *is_prime = (unsigned char*)malloc(sieve_lim + 1);
        for (size_t i = 0; i <= sieve_lim; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (size_t i = 2; i * i <= sieve_lim; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= sieve_lim; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes and store them */
        size_t prime_count = 0;
        for (size_t i = 2; i <= sieve_lim; ++i)
            if (is_prime[i]) prime_count++;

        idx_t *primes = (idx_t*)malloc(prime_count * sizeof(idx_t));
        size_t p = 0;
        for (size_t i = 2; i <= sieve_lim; ++i)
            if (is_prime[i]) primes[p++] = (idx_t)i;

        /* Allocate twin arrays (indexing starts at 1) */
        low   = (idx_t*)malloc((prime_count + 1) * sizeof(idx_t));
        high  = (idx_t*)malloc((prime_count + 1) * sizeof(idx_t));

        size_t t = 0;
        for (size_t i = 0; i + 1 < prime_count; ++i) {
            if (primes[i + 1] == primes[i] + 2) {
                ++t;
                low[t]  = primes[i];
                high[t] = primes[i + 1];
            }
        }
        twin_count = t;

        /* Clean up temporary arrays */
        free(primes);
        free(is_prime);

        initialized = true;
    }

    /* Guard against out‑of‑range requests */
    if (n == 0 || (size_t)(n + 1) > twin_count) return 0;

    /* a(n) = (lower(n+1) + higher(n)) / 2 */
    seq_t sum = (seq_t)low[(size_t)(n + 1)] + (seq_t)high[(size_t)n];
    return sum / 2;
}
