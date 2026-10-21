/*
 * A213760  Antidiagonal sums of the convolution array A213783.
 * https://oeis.org/A213760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 + 22*n^3 + (n even ? 40*n^2 + 8*n : 34*n^2 + 26*n
 *            + 12)) / 96 (OEIS A213760)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 339, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;          /* cast index to seq_t for arithmetic   */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N3 = N2 * N;            /* n^3 */
    seq_t N4 = N3 * N;            /* n^4 */

    seq_t num;
    if ((n & 1) == 0) { /* n is even */
        num = 2 * N4 + 22 * N3 + 40 * N2 + 8 * N;
    } else {            /* n is odd  */
        num = 2 * N4 + 22 * N3 + 34 * N2 + 26 * N + 12;
    }
    return num / 96;
}
