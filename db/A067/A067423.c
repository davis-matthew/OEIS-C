/*
 * A067423  Eighth column of triangle A067417.
 * https://oeis.org/A067423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0, 10 * 24^(n-1) otherwise (a recurrence a(n) =
 *            24*a(n-1) for n > 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    seq_t result = (seq_t)10;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)24;   /* a(i) = a(i-1) * 24 */

    return result;
}
