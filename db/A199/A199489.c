/*
 * A199489  a(n) = (11*7^n + 1)/6.
 * https://oeis.org/A199489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 1, a(0) = 2 */
seq_t a(idx_t n) {
    seq_t result = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)7 - (seq_t)1;
    }
    return result;
}
