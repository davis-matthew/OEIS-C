/*
 * A157818  Denominator of Bernoulli(n, 1/4).
 * https://oeis.org/A157818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(2n) * ∏{p odd prime | (p-1) divides n} p
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

seq_t a(idx_t n)
{
    /* compute 2^(2n) */
    seq_t result = pow2((idx_t)(2 * n));

    /* ensure we have all primes up to n+1 */
    if (primes_limit < n + 1)
    {
        free(primes);
        build_primes_upto(n + 1, &primes, &primes_cnt);
        primes_limit = n + 1;
    }

    /* multiply by all odd primes p with p-1 | n */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p < 3) continue;                 /* skip prime 2 */
        if (p - 1 > n) break;                /* primes are increasing */
        if (n % (p - 1) == 0)
            result = seq_mul(result, (seq_t)p);
    }

    return result;
}
