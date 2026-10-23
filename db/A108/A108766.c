/*
 * A108766  a(n) = A007290(n+2) - 1 = 2*C(n+2,3) - 1.
 * https://oeis.org/A108766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n+1)*(n+2)/3 - 1
 * Range:     i8 n <= 4, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t before any arithmetic to avoid mixing signed/unsigned types. */
    seq_t nn = (seq_t)n;                 /* n as signed sequence type */
    seq_t prod = nn * (nn + 1) * (nn + 2); /* product of three consecutive integers */
    /* The product is always divisible by 3, so integer division is exact. */
    seq_t res = prod / 3 - 1;
    return res;
}
