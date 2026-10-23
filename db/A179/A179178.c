/*
 * A179178  The number of equal-sized equilateral triangles visible (when
 *          viewed from above) in successive Genealodrons formed from 2^n -1
 *          same size equilateral triangles.
 * https://oeis.org/A179178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=7, a(4)=14, a(5)=25; for n>5, a(n) = (3*n^2
 *            - 5*n + 2)/2
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) {
        static const seq_t first[5] = {1, 3, 7, 14, 25};
        return first[n-1];
    }
    seq_t nn = (seq_t)n;
    seq_t val = ((seq_t)3 * nn * nn - (seq_t)5 * nn + (seq_t)2) / (seq_t)2;
    return val;
}
