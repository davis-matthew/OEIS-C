/*
 * A004479  Nim product 14 * n.
 * https://oeis.org/A004479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14 * n (Nim product)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[16] = {
        0, 14, 7, 9, 5, 11, 2, 12, 10, 4, 13, 3, 15, 1, 8, 6
    };
    
    if (n < 16) return table[n];
    return (a(n >> 4) << 4) | table[n & 15];
}
