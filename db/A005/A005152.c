/*
 * A005152  Rotation distance between binary trees on n nodes.
 * https://oeis.org/A005152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2n - 6 for n >= 11; precomputed values for n < 11
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[] = {0, 1, 2, 4, 5, 7, 9, 11, 12, 15};
    
    if (n <= 10) return table[n - 1];
    return (seq_t)2 * n - 6;
}
