/*
 * A152749  a(n) = (n+1)*(3*n+1)/4 for n odd, a(n) = n*(3*n+2)/4 for n even.
 * https://oeis.org/A152749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3*n+1)/4 for n odd; a(n) = n*(3*n+2)/4 for n even
 * Range:     u8 n <= 18, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;                       /* promote index to seq_t early            */
    if (n & 1)                          /* odd index                               */
        return (sn + 1) * (3 * sn + 1) / 4;
    else                                /* even index                              */
        return sn * (3 * sn + 2) / 4;
}
