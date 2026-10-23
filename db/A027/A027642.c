/*
 * A027642  Denominator of Bernoulli number B_n.
 * https://oeis.org/A027642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, core, nice
 * Formula:   a(n) = denominator of the Bernoulli number Bₙ. By the von
 *            Staudt–Clausen theorem, for n > 1 even the denominator is the
 *            product of all primes p such that (p − 1) divides n; a(0) = 1,
 *            a(1) = 2, and for odd n > 1 the denominator is 1
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   build_primes, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/seq_mul.h"

/* Static cache of primes up to the largest requested n+1. */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n & 1) return (seq_t)1;          /* odd n > 1  */

    idx_t limit = n + (idx_t)1;           /* need primes ≤ n+1 */

    /* Expand prime table if necessary. */
    if (primes == NULL || limit > primes_limit)
    {
        if (primes != NULL) free(primes);
        build_primes(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p > limit) break;
        if (n % (p - (idx_t)1) == 0)
            prod = seq_mul(prod, (seq_t)p);
    }
    return prod;
}
