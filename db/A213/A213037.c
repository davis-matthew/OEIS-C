/*
 * A213037  a(n) = n^2 - 2*floor(n/2)^2.
 * https://oeis.org/A213037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - 2*floor(n/2)^2
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;         /* convert index to seq_t */
    seq_t k   = sn / 2;          /* floor(n/2)  */
    seq_t k2  = k * k;           /* k^2         */

    if ( (n & 1) == 0 ) {
        /* n even: a(2k) = 2k^2 */
        return 2 * k2;
    } else {
        /* n odd:  a(2k+1) = 2k^2 + 4k + 1 */
        return 2 * k2 + 4 * k + 1;
    }
}
