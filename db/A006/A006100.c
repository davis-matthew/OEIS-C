/*
 * A006100  Gaussian binomial coefficient [n, 2] for q = 3.
 * https://oeis.org/A006100
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + (3^(n-1)-1)/2, a(2) = 1 (equivalently a(n) =
 *            (9^n - 4*3^n + 3)/48)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;   /* a(2) */
    seq_t s = 1;        /* (3^1 - 1)/2 */
    idx_t k;
    if (n < 2) return 0;
    for (k = 3; k <= n; k++) {
        s = s * 3 + 1;          /* (3^(k-1) - 1)/2 */
        result = result * 9 + s;
    }
    return result;
}
