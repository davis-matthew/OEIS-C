/*
 * A161939  a(n) = ((3+sqrt(2))*(4+sqrt(2))^n + (3-sqrt(2))*(4-sqrt(2))^n)/2.
 * https://oeis.org/A161939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2); a(0) = 3, a(1) = 14;
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)14;
    seq_t prev2 = (seq_t)3;
    seq_t prev1 = (seq_t)14;
    seq_t val;
    for (idx_t i = 2; i <= n; ++i) {
        val = (seq_t)8 * prev1 - (seq_t)14 * prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return prev1;
}
