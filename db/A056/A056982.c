/*
 * A056982  a(n) = 4^A005187(n). The denominators of the Landau constants.
 * https://oeis.org/A056982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 4^(n + v2(n!)) where v2(n!) = n - popcountll(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A056982: a(n) = (denominator(binomial(1/2, n)))^2 = 4^(n + v2(n!)) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute v2(n!) = n - popcount(n)  (Legendre formula for prime 2) */
    idx_t pop = __builtin_popcountll((uint64_t)n);
    idx_t e = 2 * n - pop;          /* exponent of 2 in a(n) is 2e, but a(n)=4^e */

    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= (seq_t)4;            /* multiply by 4 e times: 4^e */

    return res;
}
