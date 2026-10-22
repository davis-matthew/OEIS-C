/*
 * A004120  Expansion of (1 + x - x^5) / (1 - x)^3.
 * https://oeis.org/A004120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+11)/2 - 5 for n >= 3; a(0) = 1, a(1) = 4, a(2) = 9
 * Range:     u8 n <= 17, u16 n <= 356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 9;
    
    seq_t sn = (seq_t)n;
    return sn * (sn + 11) / 2 - 5;
}
