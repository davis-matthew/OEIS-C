/*
 * A145095  Coefficients in expansion of Eisenstein series -q*E'_6.
 * https://oeis.org/A145095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 504*n*σ₅(n) (σ₅ sums fifth powers of divisors)
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 573, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; a(0) is undefined, return 0 */
        return (seq_t)0;
    }

    /* Compute σ₅(n) = Σ_{d|n} d⁵ */
    seq_t s = sigma_k(n, 5);

    /* a(n) = 504 * n * σ₅(n) */
    s *= (seq_t)n;
    s *= (seq_t)504;

    return s;
}
