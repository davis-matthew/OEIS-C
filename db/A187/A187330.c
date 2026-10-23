/*
 * A187330  a(n) = floor((4-sqrt(5))*n); complement of A187339.
 * https://oeis.org/A187330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4 - sqrt(5))*n)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t val5 = (seq_t)5 * nn * nn;   /* 5 * n^2  */
    seq_t s = floor_sqrt(val5);       /* floor(n * sqrt(5))  */
    return 4 * nn - s - (seq_t)1;
}
