/*
 * A168631  a(n) = n^6*(n^7 + 1)/2.
 * https://oeis.org/A168631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 31, u128 n <= 970,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t1 = 1;          /* n^6 */
    for (idx_t i = 0; i < 6; ++i) {
        t1 *= tn;
    }
    seq_t t2 = 1;          /* n^7 */
    for (idx_t i = 0; i < 7; ++i) {
        t2 *= tn;
    }
    t2 += 1;               /* n^7 + 1 */
    if ((n & 1) == 0) {    /* n even – divide n^6 by 2 */
        t1 >>= 1;
    } else {               /* n odd – divide n^7+1 by 2 */
        t2 >>= 1;
    }
    return t1 * t2;
}
