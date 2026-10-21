/*
 * A229148  a(n) = n^5*(7*n+5)/2.
 * https://oeis.org/A229148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(7*n+5)/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * nn;
    seq_t term = (seq_t)7 * nn + (seq_t)5;
    if (n & 1ULL)          /* n is odd – (7*n+5) is even */
        term /= (seq_t)2;
    else                   /* n is even – n^5 is even */
        n5 /= (seq_t)2;
    return n5 * term;
}
