/*
 * A004755  Binary expansion starts 11.
 * https://oeis.org/A004755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + 2^(floor(log_2(n)) + 1)
 * Range:     u8 n <= 127, u16 n <= 9691, u32 n <= 9691, u64 n <= 9691,
 *            u128 n <= 9691, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t p = 1;
    while (p <= n_seq) {
        p <<= 1;
    }
    return n_seq + p;
}
