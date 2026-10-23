/*
 * A115054  G.f.: (x^3+6*x+2)^2/(x^2+x+1)^2.
 * https://oeis.org/A115054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (x^3 + 6*x + 2)^2 / (x^2 + x + 1)^2
 *            Recurrence: a(n) + 2*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4) =
 *            p(n) where p(0)=4, p(1)=24, p(2)=36, p(3)=4, p(4)=12, p(5)=0,
 *            p(6)=1, and p(n)=0 for n>6; a(k)=0 for k<0
 * Range:     i8 n <= 9, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ring buffer for the previous four terms:
       buf[0] = a_{i-4}, buf[1] = a_{i-3}, buf[2] = a_{i-2}, buf[3] = a_{i-1}
    */
    seq_t buf[4] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0 };
    seq_t result = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t p = (seq_t)0;
        switch (i) {
            case 0: p = 4;   break;
            case 1: p = 24;  break;
            case 2: p = 36;  break;
            case 3: p = 4;   break;
            case 4: p = 12;  break;
            case 5: p = 0;   break;
            case 6: p = 1;   break;
            default: /* p remains 0 */ break;
        }

        seq_t a_i = p;
        a_i -= (seq_t)2 * buf[3];   /* a_{i-1} */
        a_i -= (seq_t)3 * buf[2];   /* a_{i-2} */
        a_i -= (seq_t)2 * buf[1];   /* a_{i-3} */
        a_i -= buf[0];              /* a_{i-4} */

        /* shift buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = a_i;

        if (i == n) result = a_i;
    }

    return result;
}
