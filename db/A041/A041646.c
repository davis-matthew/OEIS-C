/*
 * A041646  Numerators of continued fraction convergents to sqrt(342).
 * https://oeis.org/A041646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 74*a(n-2) - a(n-4) Initial values: a(0)=18, a(1)=37,
 *            a(2)=1350, a(3)=2737
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)37;
    if (n == 2) return (seq_t)1350;
    if (n == 3) return (seq_t)2737;

    seq_t buf[4];
    buf[0] = (seq_t)18;
    buf[1] = (seq_t)37;
    buf[2] = (seq_t)1350;
    buf[3] = (seq_t)2737;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp = (seq_t)74 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = temp;
    }
    return buf[n & 3];
}
