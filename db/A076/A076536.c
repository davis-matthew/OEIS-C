/*
 * A076536  Image of n at the third step in the 3x+1 Problem: syr(3,n).
 * https://oeis.org/A076536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Collatz step applied three times to n
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* start value */
    /* first Collatz step */
    x = (x & 1) ? (x * 3 + 1) : (x >> 1);
    /* second Collatz step */
    x = (x & 1) ? (x * 3 + 1) : (x >> 1);
    /* third Collatz step */
    x = (x & 1) ? (x * 3 + 1) : (x >> 1);
    return x;
}
