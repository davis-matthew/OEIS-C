/*
 * A118521  Define sequence S_m by: initial term = m, reverse digits and add 3
 *          to get next term. Entry shows S_6. This reaches a cycle of length
 *          6 in 2 steps.
 * https://oeis.org/A118521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 6, a(n) = reverse(a(n-1)) + 3 */
static inline seq_t reverse_dec(seq_t x) {
    seq_t rev = 0;
    while (x > (seq_t)0) {
        rev = rev * 10 + (x % (seq_t)10);
        x   = x / (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          // value for offset < 1 is irrelevant
    seq_t cur = 6;
    for (idx_t i = 2; i <= n; ++i)
        cur = reverse_dec(cur) + (seq_t)3;
    return cur;
}
