/*
 * A024180  a(n) = floor((3rd elementary symmetric function of 2,3,...,n+3) /
 *          (2nd elementary symmetric function of 2,3,...,n+3) ).
 * https://oeis.org/A024180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+5)*(n^2 + 9*n + 22) / (2*(3*n^2 + 29*n + 72)))
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 476, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    seq_t N = (seq_t)n;

    /* Compute numerator: n * (n + 5) * (n^2 + 9*n + 22) */
    seq_t num = N * (N + (seq_t)5) *
                (N * N + (seq_t)9 * N + (seq_t)22);

    /* Compute denominator: 2 * (3*n^2 + 29*n + 72) = 6*n^2 + 58*n + 144 */
    seq_t den = (seq_t)6 * N * N + (seq_t)58 * N + (seq_t)144;

    /* Integer division truncates toward zero, which is floor for positives */
    return num / den;
}
