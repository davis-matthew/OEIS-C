/*
 * A042107  Denominators of continued fraction convergents to sqrt(578).
 * https://oeis.org/A042107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 1154*a(n-2) - a(n-4) for n>3 initial values: a(0)=1,
 *            a(1)=24, a(2)=1153, a(3)=27696
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)1153;
    if (n == 3) return (seq_t)27696;

    /* circular buffer of the last four computed values */
    seq_t buf[4];
    buf[0] = (seq_t)1;      /* a(0) */
    buf[1] = (seq_t)24;     /* a(1) */
    buf[2] = (seq_t)1153;   /* a(2) */
    buf[3] = (seq_t)27696;  /* a(3) */

    /* iterate from n = 4 up to the requested index */
    for (idx_t i = 4; i <= n; ++i) {
        /* recurrence: a(i) = 1154*a(i-2) - a(i-4) */
        seq_t term = (seq_t)1154 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = term;  /* store the new value in the circular buffer */
    }

    /* the desired term is now at position n mod 4 */
    return buf[n & 3];
}
