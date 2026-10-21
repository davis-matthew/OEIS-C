/*
 * A062545  Continued fraction for the 2nd du Bois-Reymond constant.
 * https://oeis.org/A062545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, easy, nonn
 * Formula:   a(0)=0; a(n)=2*n+3 for n>=1
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return (seq_t)n * 2 + 3;
}
