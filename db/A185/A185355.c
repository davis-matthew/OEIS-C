/*
 * A185355  Number of n X n symmetric (0,1)-matrices containing four ones.
 * https://oeis.org/A185355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n-1)*(5*n-7)/12
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 202, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t tmp = nn * nn;
    tmp *= (nn - 1);
    tmp *= ((seq_t)5 * nn - 7);
    return tmp / 12;
}
