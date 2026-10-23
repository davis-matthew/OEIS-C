/*
 * A118513  Define sequence S_m by: initial term = m, reverse digits and add 1
 *          to get next term. Entry shows S_13. This reaches a cycle of length
 *          9 in 15 steps.
 * https://oeis.org/A118513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 13, a(n+1) = rev_decimal(a(n)) + 1 */
/* Implements the decimal digit reversal and +1 operation */
static inline seq_t reverse_decimal(seq_t x) {
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset is 1; value undefined for n<1 */
    seq_t current = 13;
    if (n == 1) return current;
    for (idx_t i = 2; i <= n; ++i) {
        current = reverse_decimal(current) + 1;
    }
    return current;
}
