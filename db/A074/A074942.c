/*
 * A074942  a(n) = phi(n) mod 3.
 * https://oeis.org/A074942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = phi(n) mod 3
 * Range:     u8 n <= 1299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Euler's totient φ(n) can be computed by the helper phi().
       We simply return the remainder after division by 3. */
    seq_t phi_val = phi(n);
    return phi_val % (seq_t)3;
}
