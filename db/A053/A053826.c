/*
 * A053826  Dirichlet inverse of sigma_4 function (A001159).
 * https://oeis.org/A053826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e), where f(p) = -1 - p⁴, f(p²) = p⁴, f(pᵉ)
 *            = 0 for e ≥ 3. (Dirichlet inverse of σ₄)
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 213, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow4_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow4_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, harmless */
    if (n == 1) return 1;         /* special case */

    idx_t temp  = n;
    seq_t result = 1;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (exp >= 3) return 0;          /* any exponent ≥ 3 makes the term zero */

            seq_t p4 = pow4_idx(p);
            seq_t local = (exp == 1) ? (-1 - p4) : p4;  /* exp == 2 here */
            result *= local;
        }
    }

    /* Remaining prime factor, if any, has exponent 1. */
    if (temp > 1)
    {
        seq_t p4 = pow4_idx(temp);
        result *= (-1 - p4);
    }

    return result;
}
