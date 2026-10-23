/*
 * A155000  a(n) = 8*a(n-1) + 56*a(n-2), n > 2; a(0)=1, a(1)=1, a(2)=15.
 * https://oeis.org/A155000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=15; a(n)=8*a(n-1)+56*a(n-2) for n>2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u || n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)15;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)15;  /* a(2) */
    for (idx_t i = 3u; i <= n; ++i) {
        seq_t cur = (seq_t)8 * prev1 + (seq_t)56 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
