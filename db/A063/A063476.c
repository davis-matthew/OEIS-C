/*
 * A063476  a(n) = Sum_{d |C(n)} d^2, where C(n) is the Cototient function n -
 *          phi(n) (A051953).
 * https://oeis.org/A063476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{d | (n − φ(n))} d² (OEIS A063476)
 * Range:     u8 n <= 23, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n - phi(n);
    if (m == 0) {
        /* For n = 1, C(1) = 0 ⇒ no positive divisors, sum = 0 */
        return 0;
    }
    /* sigma_k returns Σ_{d | m} d^k, here k = 2 */
    return sigma_k((idx_t)m, 2);
}
