/*
 * A192744  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based implementation
   a(0) = 1, d(0) = 0
   for n >= 1:
       fact   = n!
       a(n)   = d(n-1) + fact
       d(n)   = a(n-1) + d(n-1)
   The constant term of the reduced polynomial p(n,x) is a(n). */
seq_t a(idx_t n)
{
    /* use seq_t for arbitrary precision arithmetic */
    seq_t fact = 1;          /* n!  */
    seq_t a_prev = 1;        /* a(0) */
    seq_t d_prev = 0;        /* d(0) */

    if (n == 0)
        return a_prev;

    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;                 /* update factorial */
        seq_t a_new = d_prev + fact;      /* a(n) = d(n-1) + n! */
        seq_t d_new = a_prev + d_prev;    /* d(n) = a(n-1) + d(n-1) */
        a_prev = a_new;
        d_prev = d_new;
    }
    return a_prev;
}
