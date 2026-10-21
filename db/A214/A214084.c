/*
 * A214084  Triangle read by rows: n-th row = {m: n^2 <= m <= n^3}.
 * https://oeis.org/A214084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k^2 + (n - C(k-1)) where k is the smallest integer such
 *            that C(k) = Σ_{i=0}^{k} (i^3 - i^2 + 1) > n. This implements the
 *            triangle read by rows where the n-th row contains the integers m
 *            with n^2 <= m <= n^3
 * Range:     u8 n <= 562, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t row_len(idx_t k)
{
    seq_t K = (seq_t)k;
    return K * K * K - K * K + 1;
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    while (1) {
        seq_t len = row_len(k);
        if ((seq_t)n < len) {
            seq_t K = (seq_t)k;
            return K * K + n;
        }
        n -= (idx_t)len;
        ++k;
    }
}
