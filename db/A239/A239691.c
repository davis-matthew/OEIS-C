/*
 * A239691  Base 5 sum of digits of prime(n).
 * https://oeis.org/A239691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base-5 digits of the nth prime (A239691)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, ignore n==0 */

    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t sieve_limit = 0;

    /* Estimate a safe upper bound for the nth prime: n*20 suffices for n up to 10000 */
    idx_t limit = n * 20;
    if (limit < 20) limit = 20;  /* minimal bound for very small n */

    /* Rebuild the sieve if we need more primes */
    if (n > prime_cnt) {
        if (primes) free(primes);   /* discard old array */
        build_primes_upto(limit, &primes, &prime_cnt);
        sieve_limit = limit;
    }

    /* Retrieve the nth prime (1‑based indexing) */
    uint64_t p = primes[n - 1];

    /* Compute sum of base‑5 digits */
    seq_t sum = 0;
    while (p) {
        sum += p % 5;
        p /= 5;
    }
    return sum;
}
