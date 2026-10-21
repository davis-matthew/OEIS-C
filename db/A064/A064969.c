/*
 * A064969  Number of cyclic subgroups of the group C_n X C_n X C_n (where C_n
 *          is the cyclic group of order n).
 * https://oeis.org/A064969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e || n} [1 + (p^2 + p + 1) * ((p^(2e) -
 *            1)/(p^2 - 1))]
 * Range:     u8 n <= 9, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t primes[20];
    int exps[20];
    int cnt;
    seq_t result = 1;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    for (int i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned e = exps[i];

        seq_t pp = p * p;                    /* p^2 */
        seq_t pow_p_2e = pow_seq(p, 2 * e);   /* p^(2e)  */
        seq_t numerator   = pow_p_2e - 1;     /* p^(2e)-1 */
        seq_t denom       = pp - 1;           /* p^2-1   */
        seq_t q           = numerator / denom;/* exact quotient */

        seq_t temp = pp + p + 1;             /* p^2 + p + 1 */
        seq_t ap   = 1 + temp * q;           /* 1 + (p^2+p+1)*q */

        result *= ap;
    }

    return result;   /* result is 1 for n = 1 (empty product) */
}
