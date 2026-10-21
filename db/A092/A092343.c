/*
 * A092343  a(n) = sigma_3(3n+2).
 * https://oeis.org/A092343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_3(3*n+2)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    idx_t m = (idx_t)3 * n + 2;   /* compute 3*n + 2 as idx_t */
    return sigma_k(m, 3);         /* sum of cubes of divisors of m */
}
