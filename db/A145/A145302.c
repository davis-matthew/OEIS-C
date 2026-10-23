/*
 * A145302  a(n) = ((7 + sqrt(7))^n + (7 - sqrt(7))^n)/2.
 * https://oeis.org/A145302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 42*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)7;
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)7;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t cur = (seq_t)14 * prev1 - (seq_t)42 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
