/*
 * A138557  Expansion of eta(q)^2 * eta(q^4)^2 * eta(q^10)^7 / (eta(q^2)^3 *
 *          eta(q^5)^2 * eta(q^20)^2) in powers of q.
 * https://oeis.org/A138557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 633, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pow_uint_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative formula for A138557:
   a(1) = 1
   For n > 1, let n = ∏ p_i^{e_i}. Then
     - if p = 2:      a(2^e)      = -2^e
     - if p = 5:      a(5^e)      = 5^e
     - if p ≡ 1 or 4 (mod 5):
                    a(p^e) = (p^(e+1) - 1)/(p-1)
     - if p ≡ 2 or 3 (mod 5):
                    a(p^e) = (p^(e+1) + (-1)^e)/(p+1)
   The value for n is the product of the terms for each prime power.  */
#include <stdint.h>
#include "../utils/pow_uint_custom.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;
    idx_t p, e;

    /* factor 2 separately */
    e = 0;
    while ((m & 1) == 0)
    {
        ++e;
        m >>= 1;
    }
    if (e > 0)
    {
        seq_t term = pow_uint_custom(2, e);
        term = -term;
        result *= term;
    }

    /* factor odd primes */
    for (p = 3; p <= m / p; p += 2)
    {
        if (m % p != 0) continue;
        e = 0;
        while (m % p == 0)
        {
            ++e;
            m /= p;
        }
        seq_t term;
        if (p == 5)
        {
            term = pow_uint_custom(5, e);
        }
        else
        {
            idx_t mod5 = p % 5;
            seq_t power = pow_uint_custom(p, e + 1); /* p^(e+1) */
            if (mod5 == 1 || mod5 == 4) /* p ≡ 1 or 4 (mod 5) */
            {
                seq_t numerator = power - 1;
                seq_t denominator = p - 1;
                term = numerator / denominator;
            }
            else /* mod5 == 2 or 3 */
            {
                seq_t numerator = power + ((e & 1) ? -1 : 1);
                seq_t denominator = p + 1;
                term = numerator / denominator;
            }
        }
        result *= term;
    }

    /* remaining prime factor, if any */
    if (m > 1)
    {
        seq_t term;
        if (m == 5)
        {
            term = pow_uint_custom(5, 1);
        }
        else
        {
            idx_t pprime = m;
            idx_t mod5 = pprime % 5;
            seq_t power = pow_uint_custom(pprime, 2); /* p^(1+1) */
            if (mod5 == 1 || mod5 == 4)
            {
                seq_t numerator = power - 1;
                seq_t denominator = pprime - 1;
                term = numerator / denominator;
            }
            else
            {
                seq_t numerator = power + 1; /* (-1)^1 = -1, so +(-1) = -1, add -1 => -1? actually e=1 gives -1, so numerator = power -1, but formula uses +(-1)^e. For e=1: +(-1) = -1, so power -1. */
                /* Correction: for e=1, power = p^2, (-1)^1 = -1, so numerator = p^2 - 1 */
                numerator = power - 1;
                seq_t denominator = pprime + 1;
                term = numerator / denominator;
            }
        }
        result *= term;
    }

    return result;
}
