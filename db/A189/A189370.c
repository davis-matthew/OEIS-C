/*
 * A189370  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(2), t=sqrt(5).
 * https://oeis.org/A189370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute floor(n*sqrt(2)) as floor(sqrt(2*n*n)) */
    seq_t prod2 = (seq_t)2 * nn * nn;
    seq_t f2 = floor_sqrt(prod2);

    /* Compute floor(n*sqrt(5)) as floor(sqrt(5*n*n)) */
    seq_t prod5 = (seq_t)5 * nn * nn;
    seq_t f5 = floor_sqrt(prod5);

    /* Return the sum */
    return nn + f2 + f5;
}
