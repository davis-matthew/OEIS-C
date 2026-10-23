/*
 * A101990  a(1) = a(2) = 1, a(3) = 9; for n > 3, a(n) = 3*a(n-1) - 3*a(n-2) +
 *          9*a(n-3).
 * https://oeis.org/A101990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=9; a(n)=3*a(n-1)-3*a(n-2)+9*a(n-3) for n>3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;

    seq_t prev3 = (seq_t)1;   /* a(1) */
    seq_t prev2 = (seq_t)1;   /* a(2) */
    seq_t prev1 = (seq_t)9;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = 3 * prev1 - 3 * prev2 + 9 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
