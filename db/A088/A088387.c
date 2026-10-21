/*
 * A088387  Prime corresponding to largest prime power factor of n, a(1)=1.
 * https://oeis.org/A088387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = prime p such that p^e is the largest prime power factor
 *            in the factorisation of n, with a(1)=1. It uses factor_uint64 to
 *            factor n efficiently
 * Range:     u8 n <= 255, u16 n <= 8926, u32 n <= 8926, u64 n <= 8926,
 *            u128 n <= 8926, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factor_uint64 works on uint64_t, so cast. */
    uint64_t num = (uint64_t)n;
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;

    factor_uint64(num, fac, exp, &cnt);

    seq_t max_pow = (seq_t)1;
    seq_t best_pr = (seq_t)1;

    for (int i = 0; i < cnt; ++i)
    {
        seq_t pow = (seq_t)1;
        for (int j = 0; j < exp[i]; ++j)
            pow *= (seq_t)fac[i];

        if (pow > max_pow)
        {
            max_pow = pow;
            best_pr = (seq_t)fac[i];
        }
    }

    return best_pr;
}
