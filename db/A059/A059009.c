/*
 * A059009  Numbers having an odd number of zeros in their binary expansion.
 * https://oeis.org/A059009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0)=0; a(2*n) = -a(n)+6*n+1; a(2*n+1) = a(n)+2*n+2
 * Range:     u8 n <= 127, u16 n <= 9296, u32 n <= 9296, u64 n <= 9296,
 *            u128 n <= 9296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((n & 1) == 0) {                         /* n even: n = 2*k */
        idx_t k = n >> 1;
        seq_t v = a(k);                         /* a(k) */
        seq_t t = (seq_t)6 * k + 1;             /* 6*k + 1   */
        return t - v;                           /* (6*k+1)-a(k) */
    } else {                                    /* n odd: n = 2*k+1 */
        idx_t k = n >> 1;
        seq_t v = a(k);                         /* a(k) */
        seq_t t = (seq_t)2 * k + 2;             /* 2*k + 2   */
        return v + t;                           /* a(k)+2*k+2 */
    }
}
