/*
 * A101321  Table T(n,m) = 1 + n*m*(m+1)/2 read by antidiagonals: centered
 *          polygonal numbers.
 * https://oeis.org/A101321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 + r*c*(c+1)/2 where r = n - D*(D+1)/2, c = D - r, D =
 *            largest integer with D*(D+1)/2 <= n. (Antidiagonal reading of T)
 *            Sequence A101321. See OEIS entry for full description
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find D so that D*(D+1)/2 <= n < (D+1)*(D+2)/2 */
    idx_t D = 0;
    idx_t triSum = 0;          /* current triangular sum S(D) = D*(D+1)/2 */
    while (triSum + (D + 1) <= n) {
        triSum += D + 1;
        ++D;
    }
    /* Offset within the D‑th antidiagonal */
    idx_t r = n - triSum;          /* row index */
    idx_t c = D - r;               /* column index */

    /* Compute 1 + r*c*(c+1)/2 with unsigned operations */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)c;
    seq_t val = (seq_t)1 + sr * sc * (sc + (seq_t)1) / (seq_t)2;
    return val;
}
