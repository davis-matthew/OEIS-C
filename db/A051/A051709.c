/*
 * A051709  a(n) = sigma(n) + phi(n) - 2n.
 * https://oeis.org/A051709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + phi(n) - 2*n
 * Range:     u8 n <= 239, u16 n <= 9206, u32 n <= 9206, u64 n <= 9206,
 *            u128 n <= 9206, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Undefined for n = 0, return 0 */

    seq_t sigma_val = sigma_of(n);       /* Sum of divisors of n */
    seq_t phi_val   = phi(n);            /* Euler totient of n */

    seq_t result = sigma_val;
    result += phi_val;                   /* result = sigma + phi */
    result -= (seq_t)n * (seq_t)2;       /* subtract 2*n */

    return result;
}
