/*
 * A100150  Structured snub cubic numbers.
 * https://oeis.org/A100150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(38*n^3 - 48*n^2 + 16*n)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 558, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */

    /* Compute the polynomial */
    seq_t term = 38 * n3 - 48 * n2 + 16 * N;

    /* Divide by 6 (the result is an integer for all n >= 1) */
    return term / 6;
}
