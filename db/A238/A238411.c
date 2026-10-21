/*
 * A238411  a(n) = 2*n*floor(n/2).
 * https://oeis.org/A238411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*floor(n/2) (equivalently a(n) = n^2 for n even, a(n)
 *            = n*(n-1) for n odd)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t before any arithmetic */
    seq_t nn = (seq_t)n;
    if (nn & 1)          /* odd n */
        return nn * (nn - 1);
    else                 /* even n */
        return nn * nn;
}
