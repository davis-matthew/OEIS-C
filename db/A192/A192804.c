/*
 * A192804  Constant term in the reduction of the polynomial 1+x+x^2+...+x^n
 *          by x^3->x^2+x+1. See Comments.
 * https://oeis.org/A192804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1 * Recurrence: a(n) =
 *            a(n-1)+a(n-2)+a(n-3)-1, for n >= 3
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n < 3)
        return (seq_t)1;

    /* Keep the last three values: a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)1;   /* a(2) */
    seq_t an = a0;         /* will hold a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 + a1 + a0 - (seq_t)1;
        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
