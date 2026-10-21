/*
 * A069193  a(n) = Sum_{d|n} d*phi(n)/phi(d).
 * https://oeis.org/A069193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = Sum_{d|n} d * phi(n) / phi(d) (OEIS A069193)
 * Range:     u8 n <= 47, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq_u64.h"

/* --------- computation of a(n) --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;              /* not used; safety */

    idx_t temp = n;
    seq_t result = 1;

    /* factor n by simple trial division (safe for any 64‑bit n) */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p != 0U)
            continue;

        /* count exponent e for prime p */
        idx_t e = 0;
        while (temp % p == 0U)
        {
            temp /= p;
            ++e;
        }

        /* a(p^e) = ((e+1)*p - 1) * p^(e-1) */
        seq_t base = (seq_t)p;
        seq_t coeff = (seq_t)(((e + 1U) * p) - 1U);      /* (e+1)*p - 1 */
        seq_t powp = pow_seq_u64(base, e - 1U);          /* p^(e-1) */
        seq_t term = coeff * powp;

        result *= term;
    }

    /* if temp > 1, it is a remaining prime factor of exponent 1 */
    if (temp > 1U)
    {
        seq_t term = (seq_t)((2U * temp) - 1U);          /* e = 1: ((1+1)*p - 1) */
        result *= term;
    }

    return result;
}
