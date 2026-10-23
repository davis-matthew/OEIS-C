/*
 * A119583  a(1)=0, a(2)=0, a(3)=0, a(4)=1 then
 *          a(n)=abs(a(n-1)-2*a(n-2)+a(n-3))-a(n-4).
 * https://oeis.org/A119583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=0, a(2)=0, a(3)=0, a(4)=1 and for n>=5:
 *            a(n)=abs(a(n-1)-2*a(n-2)+a(n-3))-a(n-4)
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 4)
    {
        static const seq_t base[5] = {0, 0, 0, 0, 1}; /* indices 0..4 */
        return base[n];
    }

    /* Sliding window: a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = 0;    /* a(1)   */
    seq_t a2 = 0;    /* a(2)   */
    seq_t a3 = 0;    /* a(3)   */
    seq_t a4 = 1;    /* a(4)   */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t t = a4 - (seq_t)2 * a3 + a2;           /* a(n-1)-2*a(n-2)+a(n-3) */
        seq_t abs_t = (t >= 0) ? t : -t;             /* |…| */
        seq_t next = abs_t - a1;                     /* –a(n-4) */

        /* slide the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
