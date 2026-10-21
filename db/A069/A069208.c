/*
 * A069208  a(n) = Sum_{ d divides n } phi(n)/phi(d).
 * https://oeis.org/A069208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(n)/phi(d) Equivalent multiplicative form:
 *            a(p^e) = (p^(e+1)-p^e+p^(e-1)-1)/(p-1) for each prime power p^e
 *            || n and a(n) is the product of these values over all prime
 *            powers
 * Range:     u8 n <= 16, u16 n <= 2852, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* not defined for 0, guard only */

    uint64_t temp = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(temp, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* compute p^e */
        seq_t pow_pe = 1;
        for (uint64_t j = 0; j < e; ++j)
            pow_pe *= (seq_t)p;

        /* compute p^(e+1) and p^(e-1) */
        seq_t pow_p_eplus1 = pow_pe * (seq_t)p;
        seq_t pow_p_emin1 = pow_pe / (seq_t)p;   /* e>=1, gives p^(e-1) */

        /* numerator and denominator of the factor */
        seq_t term_num = pow_p_eplus1 - pow_pe + pow_p_emin1 - (seq_t)1;
        seq_t term = term_num / (seq_t)(p - 1);

        result *= term;
    }

    return result;
}
