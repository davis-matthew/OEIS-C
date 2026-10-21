/*
 * A078439  a(n) = Sum_{k=1..n} gcd(k,n)*mu(gcd(k,n))^2.
 * https://oeis.org/A078439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Sum_{d|n} d * mu(d)^2 * phi(n/d) For prime powers: a(p) =
 *            2*p - 1 a(p^e) = 2*(p-1)*p^(e-1) for e > 1 a(n) is
 *            multiplicative
 * Range:     u8 n <= 65, u16 n <= 4289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1, but guard */

    /* Factor n using the provided helper */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;
    _factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p          = (seq_t)pr[i];
        uint64_t e       = exp[i];
        seq_t term;

        if (e == 1) {
            /* a(p) = 2*p - 1 */
            term = (seq_t)2 * p - 1;
        } else {
            /* compute p^(e-1) */
            seq_t power = 1;
            for (uint64_t k = 1; k < e; ++k)
                power *= p;
            /* a(p^e) = 2*(p-1)*p^(e-1) */
            term = (seq_t)2 * (p - 1) * power;
        }

        result *= term;
    }

    /* For n == 1 (cnt == 0) the loop is skipped, result remains 1 */
    return result;
}
