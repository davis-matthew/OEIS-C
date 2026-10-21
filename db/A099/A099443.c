/*
 * A099443  A Chebyshev transform of Fib(n+1).
 * https://oeis.org/A099443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = {1,1,1,1,0,-1,-1,-1,-1,0}[n mod 10] (period 10)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table for one period of the sequence */
    static const seq_t tbl[10] = {1, 1, 1, 1, 0, -1, -1, -1, -1, 0};
    return tbl[n % 10];
}
