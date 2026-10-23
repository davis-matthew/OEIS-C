/*
 * A155117  a(n) = 4*a(n-1) + 4*a(n-2), n>2, a(0)=1, a(1)=3, a(2)=15.
 * https://oeis.org/A155117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2), a(0)=1, a(1)=3, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;

    seq_t prev2 = (seq_t)3;  /* a(1) */
    seq_t prev1 = (seq_t)15; /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)4 * prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
