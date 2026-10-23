/*
 * A185601  a(n) = floor(n^(3/2))-floor(n^(1/2))-n; complement of A185602.
 * https://oeis.org/A185601
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2)) - floor(n^(1/2)) - n
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 555, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3; for smaller n the value is irrelevant. */
    if (n < 3)
        return 0;

    seq_t nseq = (seq_t)n;
    /* floor(n^(3/2)) == floor(sqrt(n^3)) */
    seq_t n2   = nseq * nseq;          /* n^2   */
    seq_t n3   = n2   * nseq;          /* n^3   */
    seq_t fn3  = floor_sqrt(n3);       /* floor sqrt(n^3) */

    /* floor(n^(1/2)) */
    seq_t fn1  = floor_sqrt(nseq);      /* floor sqrt(n) */

    return fn3 - fn1 - nseq;
}
