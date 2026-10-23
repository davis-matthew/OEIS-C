/*
 * A167563  The sixth row of the ED2 array A167560.
 * https://oeis.org/A167563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n^5 + 20*n^4 + 202*n^3 + 124*n^2 + 368*n
 * Range:     u8 none, u16 n <= 5, u32 n <= 58, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type first to keep the expression
       strictly in terms of seq_t. The series is defined for n >= 1,
       but the formula works for any non‑negative n. */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;           /* n^2 */
    seq_t N3 = N2 * N;          /* n^3 */
    seq_t N4 = N3 * N;          /* n^4 */
    seq_t N5 = N4 * N;          /* n^5 */

    /* Assemble the polynomial term by term. All intermediate results
       are computed in seq_t, so the function automatically adapts to
       8, 16, 32, 64, 128‑bit unsigned integers or arbitrary‑precision
       types as supplied by the build. */
    seq_t result = 0;
    result += 6 * N5;
    result += 20 * N4;
    result += 202 * N3;
    result += 124 * N2;
    result += 368 * N;
    return result;
}
