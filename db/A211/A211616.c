/*
 * A211616  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and -2<=w+x+y<=2.
 * https://oeis.org/A211616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(1) = 6; for n ≥ 2, a(n) = 15*n^2 - 15*n + 12
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 6;
    seq_t nn = (seq_t)n;
    return 15 * nn * nn - 15 * nn + 12;
}
