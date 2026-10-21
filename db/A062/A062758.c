/*
 * A062758  Product of squares of divisors of n.
 * https://oeis.org/A062758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^τ(n) (τ(n) = number of divisors of n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 59, u128 n <= 179,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides: void factor_u64(uint64_t n,
                                    uint64_t *pr, uint64_t *exp, size_t *cnt); */

seq_t a(idx_t n)
{
    /* OEIS offset 1 – for safety return 0 when n==0. */
    if (n == 0)
        return 0;

    /* factor the integer n (work with 64‑bit value – enough for all tests) */
    uint64_t pr[32], exp[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    /* compute τ(n) = ∏ (exp_i + 1)  */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exp[i] + 1);

    /* compute n^τ(n) – a simple loop is fast enough as τ(n) is small. */
    seq_t base   = (seq_t)n;
    seq_t result = 1;
    for (idx_t i = 0; i < tau; ++i)
        result *= base;

    return result;
}
