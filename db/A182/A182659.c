/*
 * A182659  A canonical permutation designed to thwart a certain naive attempt
 *          to guess whether sequences are permutations.
 * https://oeis.org/A182659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n>0, * a(n) = (n-1)/2 if n+3 is divisible by 6 and
 *            (n+3)/6 is a power of two, * otherwise a(n) = n+1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t m = n + 3;                     /* n+3 */
    if ((m % 6) == 0)                    /* divisible by 6 */
    {
        idx_t t = m / 6;                 /* t = (n+3)/6  */
        if (t && ((t & (t - 1)) == 0))   /* power of two? */
            return ((seq_t)n - 1) / 2;
    }
    return (seq_t)n + 1;
}
