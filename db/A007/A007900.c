/*
 * A007900  Coordination sequence for D_4 lattice.
 * https://oeis.org/A007900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n, 4)} p_k * C(n-k+3, 3) where p = [1, 20,
 *            54, 20, 1] and C(m, 3) = m(m-1)(m-2)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 645, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p[5] = {1, 20, 54, 20, 1};
    seq_t result = 0;
    for (int k = 0; k < 5 && k <= n; ++k) {
        idx_t m = n - k + 3;
        seq_t m_seq = (seq_t)m;
        seq_t binom = m_seq * (m_seq - 1) * (m_seq - 2) / 6;
        result += p[k] * binom;
    }
    return result;
}
