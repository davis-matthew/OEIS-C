/*
 * A214345  Interleaved reading of A073577 and A053755.
 * https://oeis.org/A214345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { n^2+4*n+5 , if n even n^2+4*n+2 , if n odd }
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 9561, u64 n <= 9561,
 *            u128 n <= 9561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) /* even */
        return nn * nn + (seq_t)4 * nn + (seq_t)5;
    else                /* odd */
        return nn * nn + (seq_t)4 * nn + (seq_t)2;
}
