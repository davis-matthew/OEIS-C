/*
 * A115271  Partial sums of floor((n+4)/4)^2.
 * https://oeis.org/A115271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} floor((k+4)/4)^2
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* tmp = n + 4 (mod 2^64) */
    idx_t tmp = n + 4ULL;

    /* jmax = floor((n+4)/4) */
    idx_t jmax = tmp >> 2;

    /* r = (n+4) mod 4, tail_count = r + 1 */
    idx_t r = tmp & 3ULL;
    idx_t tail_count = r + 1ULL;

    /* m = jmax - 1 (full groups count) */
    idx_t m = (jmax > 0) ? jmax - 1 : 0;

    /* Compute S = m(m+1)(2m+1)/6 in seq_t to avoid overflow */
    seq_t m_seq = (seq_t)m;
    seq_t S = m_seq * (m_seq + 1) * ((seq_t)2 * m_seq + 1) / 6;

    /* sum = 4 * S + jmax^2 * tail_count */
    seq_t sum = (seq_t)4 * S;
    seq_t jmax_seq = (seq_t)jmax;
    sum += jmax_seq * jmax_seq * (seq_t)tail_count;

    return sum;
}
