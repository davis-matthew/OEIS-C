/*
 * A091004  Expansion of x*(1-x)/((1-2*x)*(1+3*x)).
 * https://oeis.org/A091004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091004:  Expansion of x*(1-x)/((1-2*x)*(1+3*x)).
   Recurrence: a(0)=0, a(1)=1, a(2)=-2, and for n>=3
       a(n) = -a(n-1) + 6*a(n-2)                     */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-2;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)-2;  /* a(2) */
    seq_t cur;
    for (idx_t i = 2; i < n; ++i) {
        cur = -(prev1) + (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
