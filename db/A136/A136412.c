/*
 * A136412  a(n) = (5*4^n + 1)/3.
 * https://oeis.org/A136412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*4^n + 1)/3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2; for n > 0, a(n) = 4*a(n-1) - 1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    seq_t val = (seq_t)2;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i)      /* apply recurrence n times */
    {
        val = 4 * val - (seq_t)1;
    }
    return val;
}
