/*
 * A200724  Expansion of 1/(1-35*x+x^2).
 * https://oeis.org/A200724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 35, a(n) = 35*a(n-1) - a(n-2) for n ≥ 2 */
seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 35;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 35;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 35 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
