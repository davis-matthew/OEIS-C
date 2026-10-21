/*
 * A098849  a(n) = n*(n + 16).
 * https://oeis.org/A098849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 16)
 * Range:     u8 n <= 9, u16 n <= 248, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;        /* convert index to sequence type */
    return nn * (nn + (seq_t)16);
}
