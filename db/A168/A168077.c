/*
 * A168077  a(2n) = A129194(2n)/2; a(2n+1) = A129194(2n+1).
 * https://oeis.org/A168077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2*(5 - 3*(-1)^n)/8. This simplifies to a(n) = n^2 if n
 *            is odd, otherwise a(n) = n^2/4
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0)          /* even n */
        return (nn * nn) / 4;
    else                            /* odd n */
        return nn * nn;
}
