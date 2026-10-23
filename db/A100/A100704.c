/*
 * A100704  a(n) = (a(n-1) + a(n-2)) * a(n-1) * a(n-2) with a(0)=1 and a(1)=1.
 * https://oeis.org/A100704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-2)) * a(n-1) * a(n-2); a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)1; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (prev + prevprev) * prev * prevprev;
        prevprev = prev;
        prev     = curr;
    }
    return prev;
}
