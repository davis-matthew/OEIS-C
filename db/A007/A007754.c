/*
 * A007754  Array (a frieze pattern) defined by a(n,k) = (a(n-1,k)*a(n-1,k+1)
 *          - 1) / a(n-2,k+1), read by antidiagonals.
 * https://oeis.org/A007754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   T(n,k) = (n+k)*T(n-1,k) - T(n-2,k), T(0,k) = 1, T(-1,k) = 0;
 *            a(m) = T(n,k) where m = d*(d+1)/2 + n, k = d - n (antidiagonals
 *            by row n)
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 115, u64 n <= 249,
 *            u128 n <= 660, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t idx) {
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= idx) d++;

    idx_t n = idx - d * (d + 1) / 2;
    idx_t k = d - n;

    seq_t prev2 = 0;   /* T(i-2,k) */
    seq_t prev1 = 1;   /* T(i-1,k) */
    for (idx_t i = 1; i <= n; i++) {
        seq_t cur = (seq_t)(i + k) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
