/*
 * A209275  a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6 + 7*n^7 + 8*n^8 +
 *          9*n^9.
 * https://oeis.org/A209275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6 + 7*n^7 + 8*n^8
 *            + 9*n^9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 108, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;                      /* constant term */
    seq_t pow    = (seq_t)n;               /* start with n^1 */
    for (int k = 2; k <= 9; ++k) {
        pow *= (seq_t)n;                   /* pow = n^k */
        result += (seq_t)k * pow;          /* add k * n^k */
    }
    return result;
}
