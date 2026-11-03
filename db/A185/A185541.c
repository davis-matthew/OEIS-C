/*
 * A185541  a(n) = m*(m+1)/2, where m = floor(n^(3/2)).
 * https://oeis.org/A185541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m*(m+1)/2 where m = floor(n^(3/2))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9697 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n-th term (offset 1) of OEIS A185541. */
#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Convert n to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute n^3 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* m = floor(sqrt(n^3)) = floor(n^(3/2)) */
    seq_t m = isqrt_seq(n3);

    /* a(n) = m*(m+1)/2 */
    return (m * (m + 1)) / 2;
}
