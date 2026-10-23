/*
 * A047873  a(n) = max_{r=1..n-1} ceiling(t(t(n)-t(r-1))/(n-r+1)), where t() =
 *          triangular numbers A000217.
 * https://oeis.org/A047873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{r=1..n-1} ceil( t(t(n)-t(r-1)) / (n-r+1) ), t(k) =
 *            k*(k+1)/2
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 1;
    }
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t best = 0;
    for (idx_t r = 1; r < n; ++r) {
        seq_t tr1 = (seq_t)(r - 1) * (seq_t)r / 2;
        seq_t X = tn - tr1;
        seq_t tX = X * (X + 1) / 2;
        seq_t denom = (seq_t)(n - r + 1);
        seq_t val = (tX + denom - 1) / denom;  /* ceil division */
        if (val > best) {
            best = val;
        }
    }
    return best;
}
