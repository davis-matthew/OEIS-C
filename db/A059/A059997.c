/*
 * A059997  a(n) = (n/2)*(n + 1)*(3*n + 11).
 * https://oeis.org/A059997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(n+1)/2)*(3*n + 11)
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for safety with odd n, split the division by 2 before the multiplication */
    seq_t tn = (seq_t)n;
    seq_t half;

    if ((n & 1UL) == 0)            /* n is even */
        half = (tn / 2) * (tn + 1);
    else                              /* n is odd */
        half = tn * ((tn + 1) / 2);

    seq_t term = 3 * tn + 11;        /* 3*n + 11 */
    return half * term;
}
