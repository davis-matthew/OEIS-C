/*
 * A173535  a(n) = (1/11) * (5*12^n + 6).
 * https://oeis.org/A173535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*12^n + 6)/11; recurrence: a(0)=1, a(n)=12*a(n-1)-6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = 12 * a(i-1) - 6 */
        val = val * (seq_t)12 - (seq_t)6;
    }
    return val;
}
