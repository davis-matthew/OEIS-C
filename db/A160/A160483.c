/*
 * A160483  Second right hand column of the Beta triangle A160480.
 * https://oeis.org/A160483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = -11 a(2) = 36 a(3) = -85 a(4) = 166 a(n) = -4*a(n-1) -
 *            6*a(n-2) - 4*a(n-3) - a(n-4) for n > 4 (recurrence equivalent to
 *            a(n) = (-1)^n(2n+1)^2 - a(n-1)) The implementation computes the
 *            term iteratively, using only the last four values; no dynamic
 *            allocation is required
 * Range:     i8 n <= 3, i16 n <= 28, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)-11;
    if (n == 2) return (seq_t) 36;
    if (n == 3) return (seq_t)-85;
    if (n == 4) return (seq_t)166;

    /* Initialise the sliding window with the first four values */
    seq_t t1 = (seq_t)-11;   /* a(n-4) */
    seq_t t2 = (seq_t) 36;   /* a(n-3) */
    seq_t t3 = (seq_t)-85;   /* a(n-2) */
    seq_t t4 = (seq_t)166;   /* a(n-1) */

    /* Compute terms from 5 up to n */
    for (idx_t k = 5; k <= n; ++k) {
        /* a(k) = -4*t4 - 6*t3 - 4*t2 - t1 */
        seq_t val = - (seq_t)4 * t4
                    - (seq_t)6 * t3
                    - (seq_t)4 * t2
                    -       t1;
        /* Shift the window: (t1,t2,t3,t4) becomes (t2,t3,t4,val) */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = val;
    }
    return t4;
}
