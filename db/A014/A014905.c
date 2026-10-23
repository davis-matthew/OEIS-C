/*
 * A014905  a(1)=1, a(n) = 21*a(n-1) + n.
 * https://oeis.org/A014905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 21*a(n-1) + n for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but safe to return 0 */

    seq_t val = 1;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * 21 + (seq_t)i;   /* compute a(i) from a(i-1) */
    }
    return val;
}
