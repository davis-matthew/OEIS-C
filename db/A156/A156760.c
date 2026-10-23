/*
 * A156760  a(n) = 5*4^n - 1.
 * https://oeis.org/A156760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*4^n - 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, a(n)=4*a(n-1)+3 */
seq_t a(idx_t n)
{
    seq_t res = 4;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        res = 4 * res + 3;
    }
    return res;
}
