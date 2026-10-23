/*
 * A179995  Generating function A(5,t)(1+t+t^2)/(1-t)^6, where A(5,t) is an
 *          Eulerian polynomial.
 * https://oeis.org/A179995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^4 + 20*n^2 + 10) for n>0, a(0)=1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t sum = ((seq_t)3) * n4 + ((seq_t)20) * n2 + ((seq_t)10);
    return nn * sum;
}
