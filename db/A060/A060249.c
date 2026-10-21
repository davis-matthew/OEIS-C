/*
 * A060249  Size of the automorphism group of the symmetric group S_n.
 * https://oeis.org/A060249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! except a(2)=1 and a(6)=2*n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;

    /* handle the two exceptional cases */
    if (n == 2)
        return (seq_t)1;
    if (n == 6)
        return res * (seq_t)2;

    /* normal case */
    return res;
}
