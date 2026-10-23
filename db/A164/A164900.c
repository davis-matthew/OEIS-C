/*
 * A164900  a(2n) = 4*n*(n+1) + 3; a(2n+1) = 2*n*(n+2) + 3.
 * https://oeis.org/A164900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 4*n*(n+1)+3; a(2n+1) = 2*n*(n+2)+3 Equivalent compact
 *            form: a(n) = (n even) ? n^2+2n+3 : (n^2+2n+3)/2
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn + (seq_t)2 * nn + (seq_t)3;
    if (n & 1)        /* odd n */
        return t >> 1;   /* divide by 2, t is guaranteed even */
    else
        return t;         /* even n */
}
