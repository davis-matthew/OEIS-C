/*
 * A156573  a(n) = 34*a(n-1) - a(n-2) - 4232 for n > 2; a(1)=529, a(2)=13225.
 * https://oeis.org/A156573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) - 4232, a(1) = 529, a(2) = 13225
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)529;
    if (n == 2) return (seq_t)13225;

    seq_t prev2 = (seq_t)529;
    seq_t prev1 = (seq_t)13225;
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)34 * prev1 - prev2 - (seq_t)4232;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
