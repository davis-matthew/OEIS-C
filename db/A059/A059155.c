/*
 * A059155  A hierarchical sequence (S(W'2{3}c) - see A059126).
 * https://oeis.org/A059155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3n+4) - 2^(n+2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_of_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_of_2.h"

seq_t a(idx_t n)
{
    /* compute the two powers of two needed for the formula */
    seq_t p1 = pow_of_2(3 * n + (idx_t)4);
    seq_t p2 = pow_of_2(n + (idx_t)2);

    return p1 - p2;
}
