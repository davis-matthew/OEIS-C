/*
 * A170798  a(n) = n^10*(n^6 + 1)/2.
 * https://oeis.org/A170798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^6+1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 267,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t) n;
    seq_t n2  = nn * nn;          /* n^2   */
    seq_t n4  = n2 * n2;          /* n^4   */
    seq_t n6  = n4 * n2;          /* n^6   */
    seq_t n10 = n6 * n4;          /* n^10  */

    if ((nn & 1) == 0) {          /* n is even: divide n^10 by 2 first */
        return (n10 >> 1) * (n6 + 1);
    } else {                      /* n is odd: divide (n^6+1) by 2 first */
        return n10 * ((n6 + 1) >> 1);
    }
}
