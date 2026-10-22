/*
 * A003833  Sectors around outside of dartboard.
 * https://oeis.org/A003833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th sector on a standard dartboard, periodic with
 *            period 20
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t dartboard[20] = {
        20, 1, 18, 4, 13, 6, 10, 15, 2, 17,
        3, 19, 7, 16, 8, 11, 14, 9, 12, 5
    };
    
    return dartboard[(n - 1) % 20];
}
