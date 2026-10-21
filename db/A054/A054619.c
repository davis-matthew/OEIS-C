/*
 * A054619  Triangle T(n,k) = Sum_{d|k} phi(d)*n^(k/d).
 * https://oeis.org/A054619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Σ_{d|k} φ(d) * n^(k/d) (triangle read row‑wise)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* find the row r so that the first element of row r
       is at index r*(r-1)/2 + 1 and the last element at r*(r+1)/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* column index k in row r (1 ≤ k ≤ r) */
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;

    seq_t result = 0;

    /* accumulate the sum over all divisors d of k */
    for (idx_t d = 1; d <= k; ++d)
    {
        if (k % d == 0)
        {
            seq_t phi_d = phi(d);                     /* φ(d)   */
            idx_t exp   = k / d;                       /* exponent */
            seq_t pow    = pow_uint((uint64_t)r, exp); /* r^(k/d) */
            result += phi_d * pow;                     /* add term */
        }
    }
    return result;
}
