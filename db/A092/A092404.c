/*
 * A092404  a(n) = phi(n) + phi(n+1).
 * https://oeis.org/A092404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) + φ(n+1) (offset = 1)
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* provides seq_t phi(idx_t n) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, below that is undefined */
    return phi(n) + phi(n + 1);
}
