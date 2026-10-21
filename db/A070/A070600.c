/*
 * A070600  Largest number with n prime factors where all factors are less
 *          than or equal to n.
 * https://oeis.org/A070600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A007917(n)^n for n > 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007917(n) is the largest prime ≤ n. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    /* This function may be called repeatedly with arbitrary n values.
       We keep a cached list of primes up to the maximum n seen so far. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_max = 0;

    if (n <= 1) {
        return (seq_t)0;
    }

    /* Grow the prime cache if necessary. */
    if (prime_max < n) {
        /* Free previous array if it exists. */
        if (primes) {
            free(primes);
            primes = NULL;
        }
        sieve_primes(n, &primes, &prime_cnt);
        prime_max = n;
    }

    /* Binary search for the largest prime ≤ n. */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= n) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    /* lo is first index > n, so the desired prime is at lo-1. */
    seq_t p = (seq_t)primes[lo - 1];
    seq_t exp = (seq_t)n;

    return big_pow(p, exp);
}
