/*
 * A071401  Rounded volume of a regular dodecahedron with edge length n.
 * https://oeis.org/A071401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n^3 * (15+7*sqrt(5))/4)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 12, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for all intermediate calculations. */
    seq_t N = (seq_t)n;

    /* n^3 */
    seq_t N3 = N * N * N;

    /* 15 * n^3   (integer part of the product). */
    seq_t term1 = (seq_t)15 * N3;

    /* 7 * n^3    (k in the expression k * sqrt(5)). */
    seq_t k = (seq_t)7 * N3;

    /* k^2 */
    seq_t k2 = k * k;

    /* 5 * k^2   = (k * sqrt(5))^2, so floor_sqrt(5 * k^2) is
       floor(k * sqrt(5)). */
    seq_t mult5 = (seq_t)5 * k2;

    /* floor(k * sqrt(5)) */
    seq_t sqrt_term = floor_sqrt(mult5);

    /* Round by adding 2 before dividing by 4 (floor((x+2)/4)). */
    return (term1 + sqrt_term + (seq_t)2) / (seq_t)4;
}
