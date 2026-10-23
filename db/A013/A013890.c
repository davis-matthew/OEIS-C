/*
 * A013890  a(n) = 19^(5*n + 1).
 * https://oeis.org/A013890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^(5*n + 1) Recurrence: a(0) = 19, a(n) = 2476099 *
 *            a(n-1) (for n > 0)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)19;                         /* a(0) */
    const seq_t factor = (seq_t)2476099;        /* 19^5 = 2476099 */
    for (idx_t i = 0; i < n; ++i)               /* multiply n times */
        x *= factor;
    return x;
}
