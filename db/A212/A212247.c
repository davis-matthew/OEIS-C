/*
 * A212247  Number of (w,x,y,z) with all terms in {1,...,n} and 3w=x+y+z+n.
 * https://oeis.org/A212247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(10*n^2+3*n+2)-9*(-1)^n+9)/48
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term1 = 10 * N * N + 3 * N + 2;
    seq_t term2 = 2 * N * term1;
    if ((n & 1) != 0) {
        term2 += 18;
    }
    return term2 / 48;
}
