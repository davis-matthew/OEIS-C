/*
 * A117088  a(n) = (11*5^n - 7)/4.
 * https://oeis.org/A117088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 7, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* iterative recurrence */
    {
        res = res * (seq_t)5 + (seq_t)7;
    }
    return res;
}
