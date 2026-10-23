/*
 * A013961  a(n) = sigma_13(n), the sum of the 13th powers of the divisors of
 *          n.
 * https://oeis.org/A013961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_13(n) – sum of the 13th powers of the divisors of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 920,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the multiplicative property:   a(p^e) = 1 + p^13 + p^26 + … + p^(13e) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but safe return */

    /* n = 1 → a(1) = 1 */
    if (n == 1)
        return 1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* Compute p^13; safe because for all n whose sigma_13 fits in seq_t,
         * p^13 itself fits in seq_t as well. */
        seq_t p13 = 1;
        for (int j = 0; j < 13; ++j)
            p13 *= p;

        /* Sum the geometric series: 1 + p^13 + p^26 + … + p^(13e) */
        seq_t term = 0;
        seq_t cur   = 1;
        for (uint64_t k = 0; k <= e; ++k) {
            term += cur;
            cur *= p13;
        }

        result *= term;
    }

    return result;
}
