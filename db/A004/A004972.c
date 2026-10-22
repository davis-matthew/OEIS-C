/*
 * A004972  a(n) = ceiling(n*phi^17), where phi is the golden ratio, A001622.
 * https://oeis.org/A004972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n*phi^17); a(0) = 0, a(n) = 3571*n + 1 for n > 0
 * Range:     u8 n <= 0, u16 n <= 18, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    return (seq_t)3571 * (seq_t)n + 1;
}
