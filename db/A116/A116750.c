/*
 * A116750  Number of permutations of length n which avoid the patterns 1234,
 *          2341, 4312.
 * https://oeis.org/A116750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 3 a(n) = (n^6 + 15*n^5 - 80*n^4 - 105*n^3 + 1519*n^2 -
 *            2430*n + 720) / 360 a(1) = 1, a(2) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 0) return (seq_t)0;          /* not used but defined for safety */

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                     /* n^2  */
    seq_t N3 = N2 * N;                    /* n^3  */
    seq_t N4 = N3 * N;                    /* n^4  */
    seq_t N5 = N4 * N;                    /* n^5  */
    seq_t N6 = N5 * N;                    /* n^6  */

    /* Positive contributions */
    seq_t pos = N6;
    pos += (seq_t)15 * N5;
    pos += (seq_t)1519 * N2;
    pos += (seq_t)720;

    /* Negative contributions */
    seq_t neg = (seq_t)80 * N4;
    neg += (seq_t)105 * N3;
    neg += (seq_t)2430 * N;

    /* The numerator is guaranteed non‑negative for n >= 3 */
    seq_t numerator = pos - neg;
    return numerator / (seq_t)360;
}
