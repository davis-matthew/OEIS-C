/*
 * A042623  Denominators of continued fraction convergents to sqrt(840).
 * https://oeis.org/A042623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=57, a(3)=58, a(n) = 58*a(n-2) - a(n-4) for
 *            n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 57;
    if (n == 3) return 58;

    /* ring buffer for the last four computed terms */
    seq_t buf[4];
    buf[0] = 1;   /* a(0) */
    buf[1] = 1;   /* a(1) */
    buf[2] = 57;  /* a(2) */
    buf[3] = 58;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 58*a(i-2) - a(i-4) */
        seq_t temp = ((seq_t)58) * buf[(i - 2) & 3];
        temp = temp - buf[(i - 4) & 3];
        buf[i & 3] = temp;
    }

    return buf[n & 3];
}
