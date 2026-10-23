/*
 * A008515  5-dimensional centered cube numbers.
 * https://oeis.org/A008515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + (n+1)^5 (OEIS A008515)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 73, u64 n <= 6207,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the underlying seq_t type. */
    seq_t x = (seq_t)n;          /* n                  */
    seq_t n2 = x * x;            /* n^2                */
    seq_t n3 = n2 * x;           /* n^3                */
    seq_t n4 = n3 * x;           /* n^4                */
    seq_t n5 = n4 * x;           /* n^5                */

    /* Compute 2*n^5 + 5*n^4 + 10*n^3 + 10*n^2 + 5*n + 1 */
    seq_t res = n5 * (seq_t)2;
    res += n4 * (seq_t)5;
    res += n3 * (seq_t)10;
    res += n2 * (seq_t)10;
    res += x * (seq_t)5;
    res += (seq_t)1;

    return res;
}
