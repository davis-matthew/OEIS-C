/*
 * A131041  a(n) = 2*a(n-1) - a(n-2) - a(n-4).
 * https://oeis.org/A131041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4); initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=-1
 * Range:     i8 n <= 12, i16 n <= 27, i32 n <= 59, i64 n <= 118,
 *            i128 n <= 241, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return directly the known initial values  */
    if (n < 4) {
        static const seq_t init[] = {1, 1, 1, -1};
        return init[n];
    }

    /* keep the last four computed values:
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)  */
    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 1;   /* a(2) */
    seq_t a3 = -1;  /* a(3) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 2 * a3 - a2 - a0;   /* a(i) = 2*a(i-1) - a(i-2) - a(i-4) */
        /* shift the window for the next step */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
