/*
 * A041505  Denominators of continued fraction convergents to sqrt(269).
 * https://oeis.org/A041505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 164*a(n-3) + a(n-6) with initial values a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=162, a(4)=329, a(5)=820
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)162;
    if (n == 4) return (seq_t)329;
    if (n == 5) return (seq_t)820;

    /* Use a circular buffer of the last six values */
    seq_t buf[6];
    buf[0] = (seq_t)1;      /* a(0) */
    buf[1] = (seq_t)2;      /* a(1) */
    buf[2] = (seq_t)5;      /* a(2) */
    buf[3] = (seq_t)162;    /* a(3) */
    buf[4] = (seq_t)329;    /* a(4) */
    buf[5] = (seq_t)820;    /* a(5) */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t val = (seq_t)164 * buf[(i - 3) % 6] + buf[(i - 6) % 6];
        buf[i % 6] = val;
    }
    return buf[n % 6];
}
