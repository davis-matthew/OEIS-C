/*
 * A098021  Positions of 0's in the zero-one sequence [n*r+2*r]-[n*r]-[2*r],
 *          where r=sqrt(2) and [ ]=floor; see A187967.
 * https://oeis.org/A098021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 2*floor(n*sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (not part of the sequence, but harmless). */
    if (n == 0)
        return (seq_t)0;

    seq_t tn = (seq_t)n;               /* cast index to seq_t */
    seq_t m  = (seq_t)2 * tn * tn;     /* m = 2*n*n  */
    seq_t f  = floor_sqrt_seq(m);      /* f = floor(sqrt(2)*n) */
    seq_t res = (seq_t)3 * tn + (seq_t)2 * f; /* a(n) = 3*n + 2*f */
    return res;
}
