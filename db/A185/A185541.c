/*
 * A185541  a(n) = m*(m+1)/2, where m = floor(n^(3/2)).
 * https://oeis.org/A185541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m*(m+1)/2 where m = floor(n^(3/2))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 1512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n-th term (offset 1) of OEIS A185541. */
#include <stdint.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* Convert n to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute n^3 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* m = floor(sqrt(n^3)) = floor(n^(3/2)) */
    seq_t m = sqrt_floor(n3);

    /* a(n) = m*(m+1)/2 */
    return (m * (m + 1)) / 2;
}
