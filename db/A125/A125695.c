/*
 * A125695  Expansion of (sqrt(1+2x+9x^2)+x-1)/(2x).
 * https://oeis.org/A125695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n)+(2*n-1)*a(n-1)+9*(n-2)*a(n-2)=0
   Initial values: a(0)=1, a(1)=2 */
seq_t a(idx_t n)
{
    /* Handle first two terms explicitly */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)2;
    }

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev  = (seq_t)2;   /* a(1) */
    seq_t a_cur   = 0;          /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* numerator of the recurrence: -( (2*i-1)*a_prev + 9*(i-2)*a_prev2 ) */
        seq_t term1 = (seq_t)(2*i - 1) * a_prev;
        seq_t term2 = (seq_t)9 * (seq_t)(i - 2) * a_prev2;
        seq_t numer = -term1 - term2;

        seq_t denom = (seq_t)(i + 1);

        a_cur = numer / denom;   /* exact division */

        /* shift for next iteration */
        a_prev2 = a_prev;
        a_prev  = a_cur;
    }

    return a_cur;
}
