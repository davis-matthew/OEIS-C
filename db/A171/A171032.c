/*
 * A171032  In the sequence of prime numbers, replace all the '6' digits with
 *          '4' and vice versa.
 * https://oeis.org/A171032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime with digits 4 and 6 swapped */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    static uint32_t *primes = NULL;
    static size_t primes_needed = 0;

    if (n == 0) {
        return (seq_t)0;
    }

    /* Ensure we have at least n primes cached */
    if ((size_t)n > primes_needed) {
        if (primes) {
            free(primes);
        }
        sieve_first_n_primes(n, &primes, &primes_needed);
    }

    /* Retrieve the n-th prime (1‑based) */
    uint32_t p = primes[n - 1];
    seq_t x = (seq_t)p;

    /* Swap all 4↔6 digits */
    seq_t result = 0;
    seq_t pow10  = 1;
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 4) {
            d = 6;
        } else if (d == 6) {
            d = 4;
        }
        result += d * pow10;
        pow10 *= 10;
        x /= 10;
    }

    return result;
}
