/*
 * A013957  a(n) = sigma_9(n), the sum of the 9th powers of the divisors of n.
 * https://oeis.org/A013957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_9(n) = Σ_{d|n} d^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    return sigma_k(n, 9u);
}
