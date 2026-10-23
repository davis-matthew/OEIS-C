/*
 * A013963  a(n) = sigma_15(n), the sum of the 15th powers of the divisors of
 *          n.
 * https://oeis.org/A013963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_15(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 19, u128 n <= 370,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    return sigma_k(n, 15);
}
