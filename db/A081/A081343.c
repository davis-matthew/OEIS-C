/*
 * A081343  a(n) = (10^n + 4^n)/2.
 * https://oeis.org/A081343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 4^n)/2; recurrence: a(n) = 14*a(n-1) - 40*a(n-2),
 *            a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
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
        return (seq_t)7;

    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)7;    /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (seq_t)(14) * a1 - (seq_t)(40) * a0;
        a0 = a1;
        a1 = next;
    }

    return a1;
}
