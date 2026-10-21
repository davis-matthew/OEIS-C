/*
 * A211624  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+2x+2y>0.
 * https://oeis.org/A211624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64*n^3 - 14*n^2 + 12*n)/16 if n is even * (64*n^3 -
 *            14*n^2 + 24*n - 10)/16 if n is odd
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 448, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to the sequence type for arithmetic */
    seq_t N = (seq_t)n;

    if ((n & 1ULL) == 0)  /* n is even */
    {
        /* numerator = 64*n^3 - 14*n^2 + 12*n */
        seq_t num = 64 * N * N * N - 14 * N * N + 12 * N;
        return num / 16;
    }
    else                 /* n is odd */
    {
        /* numerator = 64*n^3 - 14*n^2 + 24*n - 10 */
        seq_t num = 64 * N * N * N - 14 * N * N + 24 * N - 10;
        return num / 16;
    }
}
