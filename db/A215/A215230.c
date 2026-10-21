/*
 * A215230  Number of length-7 0..k arrays connected end-around, with no
 *          sequence of L<n elements immediately followed by itself (periodic
 *          "squarefree").
 * https://oeis.org/A215230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+1)*(n+2)*(n*n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (so a(1) is the first term listed above) */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t term = N * (N - (seq_t)1) * (N - (seq_t)2) *
                (N + (seq_t)1) * (N + (seq_t)2) *
                (N * N - (seq_t)2);
    return term;
}
