/*
 * A131030  Period 6: repeat [16, 7, 7, 16, 25, 25].
 * https://oeis.org/A131030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic sequence with period 6: [16, 7, 7, 16, 25, 25],
 *            offset 1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[6] = {16, 7, 7, 16, 25, 25};
    /* n >= 1 assumed; modulo handles wrap‑around */
    return table[(n - 1) % 6];
}
