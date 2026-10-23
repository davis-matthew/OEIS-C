/*
 * A168178  a(n) = n^3*(n^2 + 1)/2.
 * https://oeis.org/A168178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n^2 + 1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 595, u128 n <= 595,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    seq_t u  = n2 + 1;       /* n^2 + 1 */

    /* One of n3 or u is even; divide that one by 2 first
       to keep intermediate products within range. */
    if ((n3 & 1) == 0)
        return (n3 >> 1) * u; /* (n^3/2) * (n^2 + 1) */
    else
        return n3 * (u >> 1); /* n^3 * ((n^2 + 1)/2) */
}
