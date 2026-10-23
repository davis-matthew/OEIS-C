/*
 * A015597  a(n) = 11 a(n-1) + 5 a(n-2).
 * https://oeis.org/A015597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 a(n-1) + 5 a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)11 * prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
