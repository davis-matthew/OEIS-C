/*
 * A178895  a(n) = n OR 8n, where OR is bitwise OR.
 * https://oeis.org/A178895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n | (8n)
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return sn | (sn << 3);
}
