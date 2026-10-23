/*
 * A128880  Triangular numbers congruent to 1 or 5 mod 6.
 * https://oeis.org/A128880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^2-9*n+1 if n is even, * a(n) = 18*n^2-27*n+10 if n
 *            is odd
 * Range:     u8 n <= 4, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0)           /* even n */
    {
        return (seq_t)18 * nn * nn - (seq_t)9 * nn + (seq_t)1;
    }
    else                          /* odd n */
    {
        return (seq_t)18 * nn * nn - (seq_t)27 * nn + (seq_t)10;
    }
}
