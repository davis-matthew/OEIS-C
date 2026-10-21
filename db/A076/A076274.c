/*
 * A076274  2p-1 where p is 1 or a prime.
 * https://oeis.org/A076274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*p_n - 1 where p_n is the n-th prime (p_1 = 2)
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 1 the first prime is 2. */
    if (n == 1U) return (seq_t)1U;   /* 2*2 - 1 = 1 */

    /* k = n-1 gives the number of primes needed (1‑based index). */
    idx_t k = n - 1U;

    /* Static storage for the list of primes generated so far. */
    static uint64_t *primes = NULL;
    static idx_t primes_count = 0U;
    static size_t primes_cap = 0U;
    static uint64_t candidate = 3U;   /* next odd number to test */

    /* Initialise the prime list on the first call. */
    if (!primes) {
        primes_cap   = 16U;
        primes_count = 1U;                 /* only the prime 2 */
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2U;
    }

    /* Generate more primes until we have at least k primes. */
    while (primes_count < k) {
        /* Simple primality test using already found primes. */
        int is_prime = 1;
        for (idx_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0U) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (primes_count == primes_cap) {
                primes_cap *= 2U;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof(uint64_t));
            }
            primes[primes_count++] = candidate;
        }
        candidate += 2U;   /* skip even numbers */
    }

    /* Compute the sequence value using seq_t arithmetic. */
    seq_t prime = (seq_t)primes[k - 1U];
    seq_t result = prime * (seq_t)2U;
    result -= (seq_t)1U;
    return result;
}
