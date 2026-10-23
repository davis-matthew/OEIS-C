/*
 * A167166  a(n) = n^7 mod 16.
 * https://oeis.org/A167166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 16
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce n modulo 16 to keep intermediate values small */
    seq_t base = (seq_t)n % (seq_t)16;
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result = (result * base) % (seq_t)16;
    }
    return result;
}
