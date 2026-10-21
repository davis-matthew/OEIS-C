/*
 * A097476  a(n) = Product_{i=0..n-1} ((2i)!)^2.
 * https://oeis.org/A097476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n-1} ((2*i)!)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1; /* running product of ((2*i)!)^2 */
    seq_t f   = 1; /* (2*i)! for current i */

    for (idx_t i = 0; i < n; ++i)
    {
        /* update factorial to reach (2*i)!  */
        if (i > 0)
        {
            f *= (seq_t)(2 * i - 1); /* multiply by (2*i-1) */
            f *= (seq_t)(2 * i);     /* multiply by (2*i)   */
        }

        /* square the factorial and multiply into result */
        seq_t sq = f * f;
        res *= sq;
    }

    return res;
}
