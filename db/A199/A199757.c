/*
 * A199757  a(n) = (7*11^n+1)/2.
 * https://oeis.org/A199757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*11^n+1)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4, a(n) = 11*a(n-1) - 5 for n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)4;
    }
    seq_t value = (seq_t)4;
    for (idx_t i = 1; i <= n; ++i) {
        value = (seq_t)11 * value - (seq_t)5;
    }
    return value;
}
