/*
 * A168662  a(n) = n^7*(n^5 + 1)/2.
 * https://oeis.org/A168662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(n^5 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1722,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    seq_t n5 = n3 * n2;      /* n^5 = n^3 * n^2 */
    seq_t n7 = n5 * n2;      /* n^7 = n^5 * n^2 */

    /* handle the division by 2 safely */
    if ((n & 1) == 0) {          /* n is even: n^7 is even */
        seq_t half_n7 = n7 / 2;
        return half_n7 * (n5 + 1);
    } else {                     /* n is odd: (n^5+1) is even */
        seq_t half_n5p1 = (n5 + 1) / 2;
        return n7 * half_n5p1;
    }
}
