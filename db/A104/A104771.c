/*
 * A104771  Expansion of g.f. (1-x+x^2)/(1+x-x^3).
 * https://oeis.org/A104771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = a(n) - a(n+2); a(0)=1, a(1)=-2, a(2)=3 */
seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return 1;
    if (n == 1) return -2;
    if (n == 2) return 3;

    /* Iterate using a sliding window of the last three terms */
    seq_t a_im3 = 1;          /* a(n-3)  */
    seq_t a_im2 = -2;         /* a(n-2)  */
    seq_t a_im1 = 3;          /* a(n-1)  */
    seq_t a_i;                /* a(n)    */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_i = a_im3 - a_im1;   /* a(i) = a(i-3) - a(i-1) */

        /* Shift the window for the next iteration */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = a_i;
    }
    return a_i;
}
