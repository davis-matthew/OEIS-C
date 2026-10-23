/*
 * A041065  Denominators of continued fraction convergents to sqrt(39).
 * https://oeis.org/A041065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 50*a(n-2) - a(n-4) Initial values: a(0)=1, a(1)=4,
 *            a(2)=49, a(3)=200
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)49;
    if (n == 3) return (seq_t)200;

    /* ring buffer for the four needed previous values */
    seq_t buf[4];
    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)4;  /* a(1) */
    buf[2] = (seq_t)49; /* a(2) */
    buf[3] = (seq_t)200;/* a(3) */

    /* iterate from 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)50 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = val;
    }

    return buf[n & 3];
}
