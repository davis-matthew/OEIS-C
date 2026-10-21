/*
 * A081569  Fourth binomial transform of F(n+1).
 * https://oeis.org/A081569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 19*a(n-2); a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 5;

    seq_t prev = 1;   /* a(0) */
    seq_t curr = 5;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 9 * curr - 19 * prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
