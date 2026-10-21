/*
 * A091983  a(0) = 1, a(n) = 20*sigma[3](n).
 * https://oeis.org/A091983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 20 * sigma_3(n) for n >= 1 (sigma_3 = sum of
 *            cubes of divisors)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;
    seq_t s = sigma_k(n, 3);   /* sum of p^3 over all divisors p of n */
    return s * 20;
}
