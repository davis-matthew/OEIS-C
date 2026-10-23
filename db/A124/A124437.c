/*
 * A124437  Experience Points thresholds for levels in the pen and paper
 *          role-playing game "Das Schwarze Auge" (DSA, a.k.a. "The Dark
 *          Eye").
 * https://oeis.org/A124437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 50*n*(n-1)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for out-of-range indices */
    seq_t nn = (seq_t)n;               /* convert index to seq_t type */
    seq_t res = nn * (nn - 1);         /* compute n*(n-1) */
    return res * (seq_t)50;            /* multiply by 50 */
}
