/*
 * A217854  Product of all divisors of n, positive or negative.
 * https://oeis.org/A217854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-n)^τ(n) where τ(n) is the number of positive divisors
 *            of n
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 59, i128 n <= 179,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the OEIS offset (n starts at 1). Return 0 for n==0 as a safeguard. */
    if (n == 0) {
        return 0;
    }

    /* Factor the integer n into primes.  We can safely use arrays of size 64,
       because a 64‑bit integer has at most 64 distinct prime factors. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = product of (exponent + 1) for all prime factors. */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (seq_t)(exps[i] + 1);
    }

    /* Compute (-n)^τ(n).  The base is the signed version of n. */
    seq_t base = -( (seq_t)n );
    return big_pow(base, tau);
}
