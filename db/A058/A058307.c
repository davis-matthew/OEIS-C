/*
 * A058307  a(n) = (n+1)*a(n-1) + a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A058307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n+1)*a(n-1) + a(n-2), with a(0) = 0, a(1) = 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)(i + 1) * a1 + a0;
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
