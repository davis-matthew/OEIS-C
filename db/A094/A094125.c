/*
 * A094125  a(n) = 3*2^n + 2*3^n.
 * https://oeis.org/A094125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 2*3^n; Recurrence: a(0)=5, a(1)=12,
 *            a(n)=5*a(n-1)-6*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0u)
        return (seq_t)5;
    if (n == (idx_t)1u)
        return (seq_t)12;

    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = (idx_t)2u; i <= n; ++i)
    {
        cur = (seq_t)5 * prev1 - (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
