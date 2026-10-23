/*
 * A122471  a(n) = 7*a(n-1) - n for n>0, a(0)=1.
 * https://oeis.org/A122471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;             /* a(0) */
    if (n == 0)
        return res;

    /* Iteratively apply the recurrence a(k) = 7*a(k-1) - k */
    for (idx_t k = 1; k <= n; ++k)
    {
        res = ((seq_t)7 * res) - (seq_t)k;
    }

    return res;
}
