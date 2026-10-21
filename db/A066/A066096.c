/*
 * A066096  a(n) = floor(n*phi), where phi = (1 + sqrt(5))/2.
 * https://oeis.org/A066096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi) = floor((n + sqrt(5*n*n))/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* t = 5 * n^2  */
    seq_t t = (seq_t)5 * nn * nn;
    /* i = floor(sqrt(t)) = floor(n*sqrt(5)) */
    seq_t i = floor_sqrt(t);
    /* a(n) = floor((n + i)/2) */
    return (nn + i) / 2;
}
