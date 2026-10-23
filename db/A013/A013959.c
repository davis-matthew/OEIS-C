/*
 * A013959  a(n) = sigma_11(n), the sum of the 11th powers of the divisors of
 *          n.
 * https://oeis.org/A013959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_k(n, 11) (sum of the 11th powers of the divisors of
 *            n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    return sigma_k(n, 11);
}
