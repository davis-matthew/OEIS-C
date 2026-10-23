/*
 * A041930  Numerators of continued fraction convergents to sqrt(488).
 * https://oeis.org/A041930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A041930
 * Numerators of continued fraction convergents to sqrt(488).
 * Recurrence: a(n) = 486*a(n-2) - a(n-4),
 * a(0)=22, a(1)=243, a(2)=10714, a(3)=118097.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)22;
    if (n == 1) return (seq_t)243;
    if (n == 2) return (seq_t)10714;
    if (n == 3) return (seq_t)118097;

    /* Circular buffer for a[n-4] .. a[n-1] */
    seq_t buf[4];
    buf[0] = (seq_t)22;
    buf[1] = (seq_t)243;
    buf[2] = (seq_t)10714;
    buf[3] = (seq_t)118097;

    for (idx_t i = 4; i <= n; ++i) {
        idx_t cur  = i % 4;
        idx_t two  = (i - 2) % 4;
        idx_t four = (i - 4) % 4;
        buf[cur] = (seq_t)486 * buf[two] - buf[four];
    }

    return buf[n % 4];
}
