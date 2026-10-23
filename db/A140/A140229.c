/*
 * A140229  Binomial transform of [1, 3, 3, 1, -2, 3, -4, 5, ...].
 * https://oeis.org/A140229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3*n-4)/2 for n>=3; a(1)=1, a(2)=4
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 4;
    seq_t sn = (seq_t)n;
    return ((sn + 1) * (((seq_t)3 * sn - (seq_t)4))) / (seq_t)2;
}
