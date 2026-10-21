/*
 * A057602  a(1)=2, a(n+1) is the smallest integer > a(n) such that the
 *          smallest prime factor of a(n+1) is the largest prime factor of
 *          a(n).
 * https://oeis.org/A057602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A057602: a(1)=2; for n>1, a(n)=p_{n/2}^2 if n even, 
   a(n)=p_{(n-1)/2}*p_{(n+1)/2} if n odd (with p_k the k-th prime). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;     /* primes[0] = 2, primes[1] = 3, … */
static size_t primes_count = 0;     /* number of primes currently stored */

/* Ensure that at least `needed` primes are stored in `primes`. */
static void ensure_primes(size_t needed)
{
    if (needed <= primes_count)
        return;
    if (primes)
        free(primes);
    sieve_first_n_primes((idx_t)needed, &primes, &primes_count);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    if ((n & 1) == 0) {               /* even index */
        size_t k = (size_t)(n / 2);   /* k≥1 */
        ensure_primes(k);
        seq_t p = (seq_t)primes[k - 1];
        return p * p;
    } else {                           /* odd index > 1 */
        size_t k = (size_t)((n - 1) / 2); /* k≥1 */
        ensure_primes(k + 1);
        seq_t p1 = (seq_t)primes[k - 1];
        seq_t p2 = (seq_t)primes[k];
        return p1 * p2;
    }
}
