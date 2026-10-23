/*
 * A170774  a(n) = n^8*(n^2+1)/2.
 * https://oeis.org/A170774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(n^2+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 7643,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;      /* cast index to the sequence type */
    seq_t n2  = nn * nn;       /* n^2 */
    seq_t n4  = n2 * n2;       /* n^4 */
    seq_t n8  = n4 * n4;       /* n^8 */

    if ((nn & 1) == 0) {       /* n even – divide n^8 by 2 */
        seq_t half = n8 >> 1;  /* n^8 / 2 */
        return half * (n2 + 1);
    } else {                   /* n odd – divide (n^2+1) by 2 */
        seq_t half = (n2 + 1) >> 1;
        return n8 * half;
    }
}
