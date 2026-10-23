/*
 * A042135  Denominators of continued fraction convergents to sqrt(592).
 * https://oeis.org/A042135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 146*a(n-2) - a(n-4) for n>3 a(0)=1, a(1)=3, a(2)=145,
 *            a(3)=438
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)145;
    if (n == 3) return (seq_t)438;

    /* buffer to hold the last four computed terms */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)3;   /* a(1) */
    buf[2] = (seq_t)145; /* a(2) */
    buf[3] = (seq_t)438; /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 146*a(i-2) - a(i-4) */
        seq_t val = buf[(i - 2) & 3] * (seq_t)146 - buf[(i - 4) & 3];
        buf[i & 3] = val;
    }

    return buf[n & 3];
}
