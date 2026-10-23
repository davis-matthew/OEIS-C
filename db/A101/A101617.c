/*
 * A101617  The trinomial transform (A027907) gives powers of 3, while the
 *          trinomial transform of this sequence shift one place left gives
 *          powers of 5.
 * https://oeis.org/A101617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=1, a(2)=1, a(3)=3
 *         a(n) = -2*a(n-1) + 5*a(n-2) + 6*a(n-3) - 8*a(n-4)  for n >= 4
 * This is the recurrence obtained from the rational generating function  
 *  A(x) = (1 + 3*x - 2*x^2 - 6*x^3)/(1 + 2*x - 5*x^2 - 6*x^3 + 8*x^4).
 */
seq_t a(idx_t n)
{
    /* Handle the initial conditions directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* Use a sliding window of the last four computed terms */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)1;   /* a(n-2) */
    seq_t a3 = (seq_t)3;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = -2*a3 + 5*a2 + 6*a1 - 8*a0 */
        seq_t ai = (- (seq_t)2) * a3 + (seq_t)5 * a2 + (seq_t)6 * a1 - (seq_t)8 * a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
