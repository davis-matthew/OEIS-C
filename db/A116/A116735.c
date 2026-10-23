/*
 * A116735  Number of permutations of length n which avoid the patterns 231,
 *          1234, 4312; or avoid the patterns 312, 1234, 1432, etc.
 * https://oeis.org/A116735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1; for n >= 2, a(n) = 2*n^2 - 7*n + 8
 * Range:     u8 n <= 13, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;        /* a(0)=0 (unused), a(1)=1 */
    seq_t sn = (seq_t)n;
    return (seq_t)( 2 * sn * sn - 7 * sn + 8 );
}
