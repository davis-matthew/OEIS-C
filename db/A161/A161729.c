/*
 * A161729  a(n) =
 *          ((4+sqrt(3))*(8+2*sqrt(3))^n-(4-sqrt(3))*(8-2*sqrt(3))^n)/(2*sqrt(3)).
 * https://oeis.org/A161729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 52*a(n-2), a(0) = 1, a(1) = 16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)16;         /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)16 * prev1 - (seq_t)52 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
