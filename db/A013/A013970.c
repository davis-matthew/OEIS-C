/*
 * A013970  a(n) = sigma_22(n), the sum of the 22nd powers of the divisors of
 *          n.
 * https://oeis.org/A013970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_{22}(n) = sum_{d|n} d^22
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 56,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return sigma_k(n, 22);
}
