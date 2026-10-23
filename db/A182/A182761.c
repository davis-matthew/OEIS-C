/*
 * A182761  Beatty sequence for (7 - sqrt(5))/2.
 * https://oeis.org/A182761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(7 - sqrt(5))/2)
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t nn = (seq_t)n;

    /* floor(n*sqrt(5)) via integer square root of 5*n*n */
    seq_t sqrt5_floor = floor_sqrt(nn * nn * 5);

    /* A = 7*n - floor(n*sqrt(5)) */
    seq_t A = 7 * nn - sqrt5_floor;

    /* floor(A/2) */
    seq_t base = A >> 1;           /* since A is unsigned, right shift is div by 2 */

    /* If A is even we need to subtract 1, otherwise keep base */
    return (A & 1) ? base : (base - 1);
}
