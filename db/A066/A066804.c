/*
 * A066804  Sum of diagonal elements and those below it for a square matrix of
 *          integers, starting with 1.
 * https://oeis.org/A066804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^2 - n + 2)/6
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 222, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Protect against n == 0; the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Cast to seq_t first to avoid overflow on intermediate idx_t operations */
    seq_t N = (seq_t)n;

    /* Compute each part using seq_t arithmetic */
    seq_t term1 = N * (N + 1);          /* n*(n+1) */
    seq_t inner = 2 * N * N - N + 2;    /* 2*n^2 - n + 2 */

    /* Multiply and divide by 6 */
    seq_t prod = term1 * inner;
    seq_t result = prod / 6;

    return result;
}
