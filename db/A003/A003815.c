/*
 * A003815  a(0) = 0, a(n) = a(n-1) XOR n.
 * https://oeis.org/A003815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(n) = a(n-1) XOR n, with pattern a(4k) = 4k, a(4k+1)
 *            = 1, a(4k+2) = 4k+3, a(4k+3) = 0
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 4;
    seq_t n_seq = (seq_t)n;
    if (r == 0) return n_seq;
    if (r == 1) return 1;
    if (r == 2) return n_seq + 1;
    return 0;
}
