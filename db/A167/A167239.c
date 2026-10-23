/*
 * A167239  Number of ways to partition a 5*n X 2 grid into 5 connected
 *          equal-area regions.
 * https://oeis.org/A167239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10*n^4 + 2*n^2 + 12*n)/3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure we work in the correct integer type */
    seq_t nn  = (seq_t)n;       /* n */
    seq_t nsq = nn * nn;        /* n^2 */
    seq_t n4  = nsq * nsq;      /* n^4 */

    /* Compute the numerator: 10*n^4 + 2*n^2 + 12*n */
    seq_t numerator = (seq_t)10 * n4 + (seq_t)2 * nsq + (seq_t)12 * nn;

    /* The result is divisible by 3 for all n ≥ 1 */
    return numerator / (seq_t)3;
}
