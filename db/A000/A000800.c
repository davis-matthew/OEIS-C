/*
 * A000800  Sum of upward diagonals of Eulerian triangle.
 * https://oeis.org/A000800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} A173018(n-k, k), with the Eulerian
 *            numbers built row by row: A(0,0) = 1, A(m,k) = (k+1)*A(m-1,k) +
 *            (m-k)*A(m-1,k-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A800_MAXL 32768

static seq_t a800_row[A800_MAXL + 1];

seq_t a(idx_t n) {
    idx_t L = n / 2;
    idx_t k, m;
    seq_t result;

    if (L > A800_MAXL) L = A800_MAXL;
    for (k = 0; k <= L; k++) a800_row[k] = 0;
    a800_row[0] = 1;

    /* row m = 0 contributes A(0, n), which is 1 only when n = 0 */
    result = (n == 0) ? 1 : 0;

    for (m = 1; m <= n; m++) {
        idx_t hi = m;
        if (n - m < hi) hi = n - m;
        if (L < hi) hi = L;
        for (k = hi; k >= 1; k--) {
            a800_row[k] = (seq_t)(k + 1) * a800_row[k] + (seq_t)(m - k) * a800_row[k - 1];
        }
        if (n - m <= hi) result += a800_row[n - m];
    }
    return result;
}
