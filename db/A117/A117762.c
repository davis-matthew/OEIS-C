/*
 * A117762  a(1) = 6; for n>1, a(n) = prime(n)*(prime(n)^2 - 1)/2.
 * https://oeis.org/A117762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 6; for n>1, a(n) = prime(n) * (prime(n)^2 – 1) / 2 where
 *            prime(n) is the n‑th prime
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 308, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Make sure the prime list contains at least the first n primes. */
static void
ensure_primes(idx_t n)
{
    if (n <= (idx_t)primes_cnt)
        return;
    uint32_t *new_primes = NULL;
    size_t new_cnt = 0;
    sieve_first_n_primes(n, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
}

seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)6;

    ensure_primes(n);
    /* primes array is indexed from 0, so prime(n) is primes[n-1] */
    seq_t p = (seq_t)primes[n - 1];
    seq_t term = p * (p * p - (seq_t)1) / (seq_t)2;
    return term;
}
