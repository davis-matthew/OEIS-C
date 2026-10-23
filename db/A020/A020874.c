/*
 * A020874  Number of paths in Moebius ladder M_n.
 * https://oeis.org/A020874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n*2^n-n^3-5*n^2-11*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term   = (seq_t)10 * N * pow2(n);   /* 10*n*2^n */
    seq_t n3     = N * N * N;                 /* n^3     */
    seq_t n2     = (seq_t)5 * N * N;          /* 5*n^2   */
    seq_t n1     = (seq_t)11 * N;             /* 11*n    */
    return term - n3 - n2 - n1;
}
