/*
 * A007972  Number of permutations that are 2 "block reversals" away from
 *          12...n.
 * https://oeis.org/A007972
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-2)^4 + 6*(n-2)^3 + 11*(n-2)^2 - 12*(n-2) + 6)/6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n - 2;
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t m4 = m3 * m;
    
    seq_t numerator = m4 + 6*m3 + 11*m2 - 12*m + 6;
    return numerator / 6;
}
