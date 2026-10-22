/*
 * A004768  Binary expansion ends 001.
 * https://oeis.org/A004768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 9
 * Range:     u8 n <= 30, u16 n <= 8190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * 8 + 9;
}
