/*
 * A013955  a(n) = sigma_7(n), the sum of the 7th powers of the divisors of n.
 * https://oeis.org/A013955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_7(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the 7th powers of the divisors of n. */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    return sigma_k(n, 7);
}
