/*
 * A070719  n^7 mod 38.
 * https://oeis.org/A070719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 38
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce the base modulo 38 to keep intermediate values small. */
    seq_t base = (seq_t)n % (seq_t)38;
    /* Compute base^7 modulo 38 using repeated multiplication. */
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result = (result * base) % (seq_t)38;
    }
    return result;
}
