/*
 * A139747  a(n) = 11^n - 9^n.
 * https://oeis.org/A139747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 9^n Recurrence: a(0)=0, a(1)=2,
 *            a(n)=20*a(n-1)-99*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t prev0 = 0;   /* a(0) */
    seq_t prev1 = 2;   /* a(1) */
    seq_t cur = 0;     /* will hold a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * 20 - prev0 * 99;
        prev0 = prev1;
        prev1 = cur;
    }
    return cur;
}
