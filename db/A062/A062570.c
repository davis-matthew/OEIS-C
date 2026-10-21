/*
 * A062570  a(n) = phi(2*n).
 * https://oeis.org/A062570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = φ(2·n)
 * Range:     u8 n <= 203, u16 n <= 9119, u32 n <= 9119, u64 n <= 9119,
 *            u128 n <= 9119, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* 2·n fits in idx_t (uint64_t).  Compute φ(2·n) via the utilities. */
    idx_t m = n << 1;               /* m = 2 * n  */
    return phi(m);
}
