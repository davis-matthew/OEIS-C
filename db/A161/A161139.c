/*
 * A161139  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 16.
 * https://oeis.org/A161139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_15(n)/phi(n) Multiplicative with a(p^e) = p^(14e-14) *
 *            (p^15-1)/(p-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 521,
 *            big any n
 * Helpers:   factor_u64, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition from the OEIS entry for sequence A161139. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    if (n == 1)         /* a(1) = 1 */
        return (seq_t)1;

    /* Obtain the prime factorisation of n. */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* term1 = p^(14*(e-1))   (e == 1 => 1) */
        seq_t term1 = (seq_t)1;
        if (e > 1) {
            seq_t exp = (seq_t)(14ULL * (e - 1));
            term1 = pow_seq_large(p, exp);
        }

        /* term2 = 1 + p + p^2 + … + p^14 */
        seq_t term2 = (seq_t)0;
        seq_t powv  = (seq_t)1;
        for (int j = 0; j <= 14; ++j) {
            term2 = term2 + powv;
            powv  = powv * p;
        }

        /* Accumulate the product. */
        result = result * term1;
        result = result * term2;
    }

    return result;
}
