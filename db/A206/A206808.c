/*
 * A206808  a(n) = Sum_{0<j<n} n^3-j^3.
 * https://oeis.org/A206808
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 2*n^3 - n^2)/4 (for n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 274, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 2; return 0 for smaller inputs
       to satisfy undefined behaviour advisably */
    if (n < 2)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                     /* n^2  */
    /* m = 3*n^2 - 2*n - 1  */
    seq_t m = (seq_t)3 * n2 - (seq_t)2 * nn - (seq_t)1;
    seq_t prod = n2 * m;                    /* n^2 * (3*n^2 - 2*n - 1) */
    return prod / (seq_t)4;
}
