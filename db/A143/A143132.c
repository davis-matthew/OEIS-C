/*
 * A143132  Binomial transform of [1, 5, 15, 35, 70, 0, 0, 0, ...].
 * https://oeis.org/A143132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (552 - 1190*n + 895*n^2 - 280*n^3 + 35*n^4)/12
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t N = (seq_t)n;

    /* Compute numerator: 552 + 895*N^2 + 35*N^4 - 1190*N - 280*N^3 */
    seq_t N2 = N * N;          /* N^2 */
    seq_t N4 = N2 * N2;        /* N^4 */
    seq_t N3 = N2 * N;         /* N^3 */

    seq_t numerator = 552
                      + 895 * N2
                      + 35 * N4;

    numerator -= 1190 * N;
    numerator -= 280 * N3;

    /* Division by 12 gives the exact integer term */
    return numerator / 12;
}
