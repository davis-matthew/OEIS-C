/*
 * A103831  For even n, a(n) = n*(n+1), for odd n, a(n) = 2*n + 1.
 * https://oeis.org/A103831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? n*(n+1) : 2*n+1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0)          /* even n */
        return sn * (sn + 1);   /* n*(n+1) */
    else                       /* odd n */
        return sn * 2 + 1;     /* 2*n + 1 */
}
