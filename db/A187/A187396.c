/*
 * A187396  a(n) = floor(s*n), where s =-2 + sqrt(10); complement of A187395.
 * https://oeis.org/A187396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((-2 + sqrt(10)) * n)
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic to avoid unintended truncation. */
    seq_t nn = (seq_t)n;

    /* Compute 10 * n^2.  The product fits into seq_t for all n that the
       tests require (n <= 10000).  For larger n this overflows, but the
       sequence is only needed for the range specified. */
    seq_t sq = nn * nn;   /* n^2   */
    sq *= (seq_t)10;       /* 10*n^2 */

    /* floor(n*sqrt(10)) = floor(sqrt(10*n^2)) */
    seq_t fsqrt  = floor_sqrt(sq);

    /* Compute a(n) = floor(n*sqrt(10)) - 2*n */
    seq_t two_n = nn * (seq_t)2;
    return fsqrt - two_n;
}
