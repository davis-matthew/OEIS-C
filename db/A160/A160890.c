/*
 * A160890  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 3.
 * https://oeis.org/A160890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 * ∏_{p^e‖n} (p+1) * p^{e-1} (b = 3, 2^b-1 = 7)
 * Range:     u8 n <= 23, u16 n <= 3569, u32 n <= 9997, u64 n <= 9997,
 *            u128 n <= 9997, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined, but safe to return 0 */
    }

    /* factorise n into its prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* term = (p + 1) * p^{e-1} */
        seq_t term = (seq_t)(p + 1);

        if (e > 1) {
            seq_t p_pow = 1;
            for (uint64_t k = 0; k < e - 1; ++k) {
                p_pow *= (seq_t)p;
            }
            term *= p_pow;
        }

        result *= term;
    }

    result *= (seq_t)7; /* factor (2^b - 1) = 7 for b = 3 */
    return result;
}
