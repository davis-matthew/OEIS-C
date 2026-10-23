/*
 * A158869  Number of ways of filling a 2 X 3 X 2n hole with 1 X 2 X 2 bricks.
 * https://oeis.org/A158869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 6*a(n-1) - 3*a(n-2) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)5; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)6) * prev1 - ((seq_t)3) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
