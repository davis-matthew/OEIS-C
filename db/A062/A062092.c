/*
 * A062092  a(n) = 2*a(n-1) - (-1)^n for n > 0, a(0)=2.
 * https://oeis.org/A062092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - (-1)^n, a(0) = 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t a_prev = (seq_t)2;          /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* For odd i: a(i) = 2*a_prev + 1
           For even i: a(i) = 2*a_prev - 1   */
        if (i & 1)
            a_prev = (a_prev << 1) + (seq_t)1;
        else
            a_prev = (a_prev << 1) - (seq_t)1;
    }

    return a_prev;
}
