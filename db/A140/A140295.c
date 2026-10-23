/*
 * A140295  a(n) = a(n-1) + a(n-2) + 2a(n-3).
 * https://oeis.org/A140295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3); Initial values: a(0)=1,
 *            a(1)=-2, a(2)=4
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle known base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)4;

    /* Iterative recurrence for n >= 3 */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)-2;  /* a(n-2) */
    seq_t a2 = (seq_t)4;   /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 + a1 + (seq_t)2 * a0;  /* a(n) = a(n-1)+a(n-2)+2*a(n-3) */
        a0 = a1;   /* shift window: new a(n-3) */
        a1 = a2;   /* new a(n-2) */
        a2 = an;   /* new a(n-1) */
    }
    return an;
}
