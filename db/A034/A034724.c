/*
 * A034724  a(n) = n-th sextic factorial number divided by 4.
 * https://oeis.org/A034724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = (6*n-2) * a(n-1) (sextic factorial
 *            divided by 4)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1; behaviour below the offset is irrelevant */
        return 0;

    seq_t val = 1;          /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* factor = (6*i-2).  Use seq_t arithmetic to avoid overflow of intermediate idx_t. */
        seq_t factor = ((seq_t)6 * i) - 2;
        val *= factor;
    }
    return val;
}
