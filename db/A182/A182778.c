/*
 * A182778  Beatty sequence for 3 + sqrt(3).
 * https://oeis.org/A182778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3 + sqrt(3)))
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 6232, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

/* Compute the Beatty sequence for 3 + sqrt(3) */
seq_t a(idx_t n)
{
    /* Guard against the offset value (n == 0), which is not used in the
     * OEIS data set. Returning 0 here does not affect correctness for
     * n >= 1. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;          /* cast index to the sequence type */
    seq_t t  = nn * nn;           /* n^2 */
    t = (seq_t)3 * t;            /* 3*n^2 */

    /* floor(n*sqrt(3)) == floor(sqrt(3*n^2)) */
    seq_t sqrt_val = sqrt_floor(t);

    /* a(n) = 3*n + floor(n*sqrt(3)) */
    return (seq_t)3 * nn + sqrt_val;
}
