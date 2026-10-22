/*
 * A001544  A nonlinear recurrence: a(n) = a(n-1)^2 - 6*a(n-1) + 6, with a(0)
 *          = 1, a(1) = 7.
 * https://oeis.org/A001544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - 6*a(n-1) + 6, a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    
    seq_t prev = (seq_t)7;
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = prev * prev - (seq_t)6 * prev + (seq_t)6;
        prev = next;
    }
    return prev;
}
