/*
 * A185651  A(n,k) = Sum_{d|n} phi(d)*k^(n/d); square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A185651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = A(i,k) where (i,k) are the antidiagonal coordinates * of
 *            the square array A(i,k) = Σ_{d|i} φ(d) · k^(i/d). The function *
 *            computes the linear index n into the antidiagonal enumeration
 *            and * then evaluates the required sum using the helper functions
 *            * `phi` and `big_pow`
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 200, u64 n <= 482,
 *            u128 n <= 1207, big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s * (s + 1u)) / 2u <= n)
        ++s;
    --s;                               /* now s*(s+1)/2 <= n < (s+1)*(s+2)/2 */

    idx_t offset = n - (s * (s + 1u)) / 2u;
    idx_t i = offset;                  /* row index (first argument) */
    idx_t j = s - offset;              /* column index (second argument) */

    /* The first row and first column are all zeros. */
    if (i == 0 || j == 0)
        return 0;

    seq_t sum = 0;
    /* Iterate over all divisors d of i */
    for (idx_t d = 1; d * d <= i; ++d)
    {
        if (i % d == 0)
        {
            /* First divisor d */
            seq_t phi_d = phi(d);
            idx_t exp = i / d;
            seq_t pow_val = big_pow((seq_t)j, (seq_t)exp);
            sum += phi_d * pow_val;

            /* Second divisor i/d if distinct */
            idx_t d2 = i / d;
            if (d2 != d)
            {
                seq_t phi_d2 = phi(d2);
                idx_t exp2 = i / d2;
                seq_t pow_val2 = big_pow((seq_t)j, (seq_t)exp2);
                sum += phi_d2 * pow_val2;
            }
        }
    }

    return sum;
}
