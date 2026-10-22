/*
 * A005009  a(n) = 7*2^n.
 * https://oeis.org/A005009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)7;
    for (idx_t i = 0; i < n; i++) {
        result *= (seq_t)2;
    }
    return result;
}
