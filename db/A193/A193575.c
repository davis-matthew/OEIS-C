/*
 * A193575  T(n)^3 - n^3 where T(n) is a triangular number.
 * https://oeis.org/A193575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n(n+1)/2)^3 - n^3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 56, u64 n <= 2298,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t t  = nn * (nn + 1) / 2; /* triangular number T(n) */
    seq_t t3 = t * t * t;         /* T(n)^3  */
    seq_t n3 = nn * nn * nn;      /* n^3     */
    return t3 - n3;
}
