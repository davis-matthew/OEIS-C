/*
 * A115270  Diagonal sums of correlation triangle for floor((n+4)/4).
 * https://oeis.org/A115270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=sum_{k=0..floor(n/2)} sum_{j=0..min(k,n-2k)}
 *            floor((k-j+4)/4)*floor((n-2k-j+4)/4)
 * Range:     u8 n <= 24, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t n2k = n - 2 * k;                 /* n - 2k, non‑negative for j */
        idx_t maxj = k;
        if (n2k < maxj) maxj = n2k;            /* bounds j<=k and j<=n-2k */
        for (idx_t j = 0; j <= maxj; ++j) {
            /* floor((k-j+4)/4) and floor((n-2k-j+4)/4) are non‑negative integers */
            seq_t term1 = (seq_t)((k - j + 4) / 4);
            seq_t term2 = (seq_t)((n2k - j + 4) / 4);
            sum += term1 * term2;
        }
    }
    return sum;
}
