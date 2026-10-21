/*
 * A075520  4*prime(n) + (prime(n) mod 4).
 * https://oeis.org/A075520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*prime(n) + (prime(n) mod 4)
 * Range:     u8 n <= 18, u16 n <= 1900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"

/* Static state for prime generation */
static bool initialized = false;
static idx_t *primes = NULL;
static size_t prime_count = 0;
static idx_t current_limit = 200000;

/* Initialize (or enlarge) the prime list to contain at least n primes. */
static void ensure_primes(idx_t n)
{
    if (!initialized) {
        sieve_primes(current_limit, &primes, &prime_count);
        initialized = true;
    } else if ((idx_t)prime_count < n) {
        /* enlarge the sieve limit until we have enough primes */
        while ((idx_t)prime_count < n) {
            current_limit *= 2;
            sieve_primes(current_limit, &primes, &prime_count);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – behaviour for n<1 is undefined */
    ensure_primes(n);
    idx_t p = primes[(size_t)(n - 1)];
    seq_t res = (seq_t)4 * p + (seq_t)(p % 4);
    return res;
}
