/*
 * A238473  a(n) = binomial(5*n+8, 4)/5 for n >= 0.
 * https://oeis.org/A238473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n+8, 4)/5
 * Range:     u8 none, u16 n <= 3, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = 5*n + 8;          /* compute 5*n + 8 in the native index type */
    seq_t val = binomial(m, 4); /* binomial(5*n+8, 4)  */
    return val / 5;             /* divide by 5 to obtain a(n) */
}
