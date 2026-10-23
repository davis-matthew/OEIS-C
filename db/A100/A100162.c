/*
 * A100162  Structured disdyakis dodecahedral numbers (vertex structure 7).
 * https://oeis.org/A100162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (42*n^3 - 54*n^2 + 18*n)/6 = 7*n^3 - 9*n^2 + 3*n
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 850, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;  /* offset is 1, but return 0 for n=0 for safety */
    }
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t result = 7 * N3 - 9 * N2 + 3 * N;
    return result;
}
