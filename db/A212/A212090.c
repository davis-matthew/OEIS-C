/*
 * A212090  Number of (w,x,y,z) with all terms in {1,...,n} and w<x+y+z.
 * https://oeis.org/A212090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-2+(1+(2+23*n)*n)*n)/24
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t res = ((seq_t)23 * n4 + (seq_t)2 * n3 + n2 - (seq_t)2 * nn) / (seq_t)24;
    return res;
}
