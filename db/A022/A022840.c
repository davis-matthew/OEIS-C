/*
 * A022840  Beatty sequence for sqrt(6).
 * https://oeis.org/A022840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(6))
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is unsigned; cast to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* Compute 6 * n * n and take integer square root */
    seq_t val = nn * nn;    /* n^2 */
    val *= (seq_t)6;        /* 6*n^2 */
    return floor_sqrt(val);
}
