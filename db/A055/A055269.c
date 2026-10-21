/*
 * A055269  a(n) = 4*a(n-1) - a(n-2) + 3 with a(0)=1, a(1)=7.
 * https://oeis.org/A055269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 3, a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)7;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = prev1 * (seq_t)4 - prev2 + (seq_t)3;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
