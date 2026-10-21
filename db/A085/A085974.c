/*
 * A085974  Number of 0's in the decimal expansion of prime(n).
 * https://oeis.org/A085974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the number of 0's in the decimal expansion of prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for primes */
    static size_t primes_used  = 0;
    static size_t primes_cap   = 0;
    static uint64_t *primes     = NULL;

    /* No well‑defined values for n < 1; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Allocate initial storage and seed with the first prime. */
    if (primes == NULL) {
        primes_cap = 16384;                       /* generous start */
        primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return (seq_t)0;
        primes[0]   = 2;
        primes_used = 1;
    }

    /* Generate primes until we have at least n of them. */
    while (primes_used < n) {
        uint64_t candidate = primes[primes_used - 1] + 1;
        bool found = false;
        while (!found) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_used; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime) { found = true; break; }
            ++candidate;
        }

        /* Grow the primes array if necessary. */
        if (primes_used == primes_cap) {
            primes_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                primes_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;
            primes = tmp;
        }
        primes[primes_used++] = candidate;
    }

    /* Count zeros in the nth prime. */
    uint64_t p = primes[n - 1];
    int zeros = 0;
    do {
        if ((p % 10) == 0) ++zeros;
        p /= 10;
    } while (p);

    return (seq_t)zeros;
}
