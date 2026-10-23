/*
 * A153056  a(0)=2, a(n) = n^2+a(n-1).
 * https://oeis.org/A153056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + n(n+1)(2n+1)/6
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }

    seq_t t = (seq_t)n;
    seq_t u = t + 1;
    seq_t v = (seq_t)2 * t + 1;

    /* cancel factor 2 */
    if ((t & 1) == 0) {
        t >>= 1;
    } else if ((u & 1) == 0) {
        u >>= 1;
    } else {
        v >>= 1;
    }

    /* cancel factor 3 */
    if (t % 3 == 0) {
        t /= 3;
    } else if (u % 3 == 0) {
        u /= 3;
    } else {
        v /= 3;
    }

    /* Now t*u*v is n(n+1)(2n+1)/6 */
    seq_t res = t * u * v + (seq_t)2;
    return res;
}
