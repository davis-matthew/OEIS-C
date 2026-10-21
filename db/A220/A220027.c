/*
 * A220027  a(n) = product(i >= 0, P(n, i)^(2^i)) where P(n, i) = product(p
 *          prime, n/2^(i+1) < p <= n/2^i).
 * https://oeis.org/A220027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i≥0} P(n,i)^(2^i) where P(n,i) = ∏_{p prime, n/2^(i+1)
 *            < p ≤ n/2^i} p
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

static idx_t primes_upto_limit = 0;
static uint64_t *primes_cache   = NULL;
static size_t   primes_cnt      = 0;

static void ensure_primes_upto(idx_t n)
{
    if (n <= primes_upto_limit)
        return;
    if (primes_cache != NULL)
        free(primes_cache);
    build_primes_upto(n, &primes_cache, &primes_cnt);
    primes_upto_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    if (n >= 2)
        ensure_primes_upto(n);

    seq_t result = (seq_t)1;

    for (idx_t i = 0; (n >> i) > 0; ++i)
    {
        idx_t high = n >> i;
        idx_t low  = n >> (i + 1);       /* exclusive lower bound */

        seq_t prod = (seq_t)1;

        /* multiply all primes in (low, high] */
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            uint64_t p  = primes_cache[j];
            if ((idx_t)p > high)
                break;
            if ((idx_t)p > low)
                prod = prod * (seq_t)p;
        }

        if (prod == (seq_t)1)            /* no primes in this interval */
            continue;

        seq_t exp  = (seq_t)(1ULL << i); /* 2^i  */
        seq_t powv = big_pow(prod, exp);
        result = result * powv;
    }

    return result;
}
