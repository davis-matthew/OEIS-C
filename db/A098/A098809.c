/*
 * A098809  a(n) = 2^(n+23) - 23.
 * https://oeis.org/A098809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+23) - 23
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 41, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t exp = n + (idx_t)23;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)2;
    }
    result -= (seq_t)23;
    return result;
}
