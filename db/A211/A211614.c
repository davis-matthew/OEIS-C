/*
 * A211614  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+x+y>2.
 * https://oeis.org/A211614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 - 15*n^2 + 15*n - 12)/2 (for n > 1)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 837, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;           // n^2
    seq_t N3 = N2 * N;          // n^3

    seq_t res = (seq_t)8 * N3 - (seq_t)15 * N2 + (seq_t)15 * N - (seq_t)12;
    return res / (seq_t)2;
}
