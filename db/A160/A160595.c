/*
 * A160595  Numerator of resilience R(n) = phi(n)/(n-1), with a(1) = 1 by
 *          convention.
 * https://oeis.org/A160595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = phi(n)/gcd(phi(n), n-1) with a(1) = 1
 * Range:     u8 n <= 510, u16 n <= 9288, u32 n <= 9288, u64 n <= 9288,
 *            u128 n <= 9288, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);              /* Euler's totient of n */
    seq_t n_minus_one = (seq_t)(n - 1);  /* n-1 converted to seq_t */
    seq_t g = gcd(phi_val, n_minus_one); /* greatest common divisor */
    return phi_val / g;                  /* numerator of resilience */
}
