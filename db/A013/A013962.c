/*
 * A013962  a(n) = sigma_14(n), the sum of the 14th powers of the divisors of
 *          n.
 * https://oeis.org/A013962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_14(n), the sum of the 14th powers of all divisors
 *            of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 565,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/seq_pow.h"     /* seq_pow */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* n ≤ 2^64, 64 primes are enough for any factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum   = 1;                     /* Σ_{j=0}^{e} p^{14*j} */
        seq_t term  = 1;
        seq_t p14   = seq_pow((seq_t)primes[i], 14);

        /* Add the remaining terms of the geometric series */
        for (idx_t j = 1; j <= exps[i]; ++j) {
            term *= p14;                     /* p^{14*j} */
            sum  += term;                    /* add to sum */
        }

        result *= sum;                       /* multiply with other primes */
    }

    return result;
}
