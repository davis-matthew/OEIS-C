/*
 * A055229  Greatest common divisor of largest square dividing n and
 *          squarefree part of n.
 * https://oeis.org/A055229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, mult
 * Formula:   a(n) = gcd(A008833(n), A007913(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The only primes p with odd exponent e>1 contribute a factor p.
   For any prime p, if e is even or e=1 the contribution is 1. */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n < 2)
        return (seq_t)1;

    /* factorise n */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t   cnt;
    factor_u64(n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((exp[i] & 1U) && exp[i] > 1)   /* odd exponent > 1 */
            result *= (seq_t)pr[i];
    }
    return result;
}
