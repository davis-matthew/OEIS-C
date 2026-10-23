/*
 * A184423  a(n) = (2*n)!*(3*n)!/n!^5.
 * https://oeis.org/A184423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(2*n, n)^2 * binom(3*n, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently) a(n) = (2*n)! * (3*n)! / (n!)^5 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 automatically via binom(0,0) = 1 */
    seq_t c2 = binom(2 * n, n);   /* binom(2n, n)  */
    seq_t c3 = binom(3 * n, n);   /* binom(3n, n)  */
    return c2 * c2 * c3;
}
