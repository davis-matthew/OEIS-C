/*
 * A106233  An inverse Catalan transform of A003462.
 * https://oeis.org/A106233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 41, i64 n <= 79, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=3, a(3)=5;
   for n>=4: a(n)=4*a(n-1)-7*a(n-2)+6*a(n-3)-3*a(n-4) */

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* workbuffer for the last four terms */
    seq_t buf[4] = {0, 1, 3, 5};
    idx_t i;

    for (i = 4; i <= n; ++i) {
        seq_t val = 4 * buf[3] - 7 * buf[2] + 6 * buf[1] - 3 * buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = val;
    }
    return buf[3];
}
