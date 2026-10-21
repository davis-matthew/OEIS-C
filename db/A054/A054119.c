/*
 * A054119  a(n) = n! + (n-1)! + (n-2)!.
 * https://oeis.org/A054119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + (n-1)! + (n-2)! (offset 0) For n > 2 this is
 *            equivalent to a(n) = (n-2)! * n^2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simply computes (n-2)! from scratch for each call
   and multiplies it by n^2.  The computation is O(n) in the size of the
   integer, which satisfies the required time limits for n up to ~10000
   even with arbitrary-precision integers. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Compute (n-2)! */
    seq_t fact = (seq_t)1;
    idx_t limit = n - 2;
    for (idx_t i = 2; i <= limit; ++i)
    {
        fact *= (seq_t)i;
    }

    /* Multiply by n^2 */
    seq_t nn = (seq_t)n;
    return fact * nn * nn;
}
