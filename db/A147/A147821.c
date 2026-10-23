/*
 * A147821  Number of consistent sets of 5 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147821
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-3)*(n-2)*(n-1)*n*(n^6 + n^5 - 15*n^4 - 45*n^3 - 4*n^2
 *            + 326*n + 900))/120 Offset: 4 (a(4) = 108)
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) {
        return 0; /* below the offset; value irrelevant */
    }

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;               /* n^2 */
    seq_t N3 = N2 * N;              /* n^3 */
    seq_t N4 = N3 * N;              /* n^4 */
    seq_t N5 = N4 * N;              /* n^5 */
    seq_t N6 = N5 * N;              /* n^6 */

    /* Compute the polynomial part: n^6 + n^5 - 15*n^4 - 45*n^3 - 4*n^2 + 326*n + 900 */
    seq_t poly_pos = N6 + N5 + (seq_t)326 * N + (seq_t)900;
    seq_t poly_neg = (seq_t)15 * N4 + (seq_t)45 * N3 + (seq_t)4 * N2;
    seq_t poly = poly_pos - poly_neg;

    /* Compute the product (n-3)*(n-2)*(n-1)*n */
    seq_t fac = (N - (seq_t)3) * (N - (seq_t)2) * (N - (seq_t)1) * N;

    /* Final result */
    seq_t result = fac * poly / (seq_t)120;
    return result;
}
