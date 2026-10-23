/*
 * A164606  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 21.
 * https://oeis.org/A164606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2), a(0)=1, a(1)=21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)21;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = 10 * prev1 - 17 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
