/*
 * A115224  Number of 3 X 3 symmetric matrices over Z(n) having determinant 1.
 * https://oeis.org/A115224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 1 if n = 1, otherwise a(n) = Π_{p^e‖n} (p³ – 1) * p^(5e –
 *            3) (multiplicative, a(p^e) = (p³ – 1) * p^(5e – 3))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7138,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115224 – Number of 3 × 3 symmetric matrices over Z(n) having determinant 1 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n into distinct primes with exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p   = (seq_t)primes[i];
        idx_t   e = exps[i];          /* exponent of this prime factor */

        /* (p³ – 1) */
        seq_t p2 = p * p;              /* p² */
        seq_t p3 = p2 * p;             /* p³ */
        seq_t coef = p3 - (seq_t)1;

        /* p^(5e – 3) */
        idx_t exp = 5 * e - 3;          /* e ≥ 1 so exp ≥ 2 */
        seq_t pow = pow_uint((uint64_t)p, exp);

        result *= (coef * pow);
    }

    return result;
}
