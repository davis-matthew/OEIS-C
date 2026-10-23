/*
 * A180845  a(n) = (16^n-3^n)/13.
 * https://oeis.org/A180845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (16^n-3^n)/13
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(n) = 16*a(n-1) + 3^(n-1) for n>=1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = 0;          /* a(i-1) */
    seq_t pow3 = 1;         /* 3^(i-1) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = 16*a(i-1) + 3^(i-1) */
        res = (seq_t)((seq_t)16 * res + pow3);
        /* prepare 3^i for next iteration */
        pow3 *= (seq_t)3;
    }

    return res;
}
