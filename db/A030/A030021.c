/*
 * A030021  Theta series of 8-dimensional extremal strongly 10-modular
 *          lattice.
 * https://oeis.org/A030021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 24 * σ3(floor(n/2)) + (n mod 2) * 24 * σ1(floor(n/2)),
 *            where σ1(m) = Σ_{d|m} d and σ3(m) = Σ_{d|m} d³
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t m = n / 2;                /* floor(n/2) */
    seq_t sigma1 = (seq_t)0;
    seq_t sigma3 = (seq_t)0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            seq_t dseq = (seq_t)d;
            seq_t d3 = dseq * dseq * dseq;
            idx_t q = m / d;
            if (q == d) {           /* perfect square divisor */
                sigma1 += dseq;
                sigma3 += d3;
            } else {
                seq_t qseq = (seq_t)q;
                seq_t q3 = qseq * qseq * qseq;
                sigma1 += dseq + qseq;
                sigma3 += d3 + q3;
            }
        }
    }

    seq_t res = (seq_t)24 * sigma3;
    if (n % 2)
        res += (seq_t)24 * sigma1;
    return res;
}
