/*
 * A074741  Sum of squares of gaps between consecutive primes.
 * https://oeis.org/A074741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) = sum_{k=1..n} (prime(k+1)-prime(k))^2
   via a sieve for the first n+1 primes. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes64 = NULL;   /* array of primes */
static size_t primes_cnt64 = 0;     /* number of primes in array */

/* Ensure that primes64 contains at least n+1 primes */
static void ensure_primes(idx_t n)
{
    if (n + 1 <= primes_cnt64)
        return;                   /* already sufficient */

    /* Free old array if necessary */
    if (primes64)
    {
        free(primes64);
        primes64 = NULL;
        primes_cnt64 = 0;
    }

    /* Rough upper bound for the (n+1)th prime:
       p_n < n*(log n + log log n) for n >= 6; for small n use a safe constant. */
    idx_t limit = n * 15 + 100;     /* n=10000 => limit=150100 > 104729 */
    if (limit < 20)
        limit = 20;

    /* Build primes up to the computed limit */
    build_primes_upto(limit, &primes64, &primes_cnt64);

    /* If the limit was too small, double it */
    if (primes_cnt64 <= n)
    {
        limit *= 2;
        if (primes64)
        {
            free(primes64);
            primes64 = NULL;
            primes_cnt64 = 0;
        }
        build_primes_upto(limit, &primes64, &primes_cnt64);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, but return 0 for safety */

    /* Ensure we have the first n+1 primes */
    ensure_primes(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        uint64_t gap = primes64[i] - primes64[i - 1];
        seq_t sq   = (seq_t)gap * (seq_t)gap;
        sum += sq;
    }
    return sum;
}
