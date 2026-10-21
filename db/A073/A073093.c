/*
 * A073093  Number of prime power divisors of n.
 * https://oeis.org/A073093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{p^k‖n} k (number of prime power divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    uint64_t x = (uint64_t)n;
    uint64_t fac[20];
    int     exp[20];
    int     cnt = 0;

    factor_uint64(x, fac, exp, &cnt);   /* factor n */

    int sum = 0;
    for (int i = 0; i < cnt; ++i)
        sum += exp[i];                   /* bigOmega(n) */

    return (seq_t)(sum + 1);             /* a(n) = bigOmega(n) + 1 */
}
