/*
 * A004958  a(n) = ceiling(n*phi^3), where phi is the golden ratio, A001622.
 * https://oeis.org/A004958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^3), where phi^3 = 2 + sqrt(5)
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t n64 = (uint64_t)n;
    uint64_t result = (n64 * 305 + 71) / 72;
    return (seq_t)result;
}
