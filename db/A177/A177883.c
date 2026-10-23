/*
 * A177883  Period 6: repeat [4, 5, 7, 2, 1, 8].
 * https://oeis.org/A177883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [4,5,7,2,1,8][n mod 6]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t period[6] = {4,5,7,2,1,8};
    return period[(int)(n % 6)];
}
