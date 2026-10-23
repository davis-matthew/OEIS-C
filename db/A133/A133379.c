/*
 * A133379  Expansion of -x^2*(50*x+1)/(160264*x^3+80136*x^2-49*x-1).
 * https://oeis.org/A133379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   Recurrence: a(1)=0, a(2)=a(3)=1. For n>=4: a(n) = -49*a(n-1) +
 *            80136*a(n-2) + 160264*a(n-3);
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2 || n == 3)
        return (seq_t)1;

    seq_t a1 = 1;      /* a(n-1) */
    seq_t a2 = 1;      /* a(n-2) */
    seq_t a3 = 0;      /* a(n-3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = -((seq_t)49 * a1) + ((seq_t)80136 * a2)
                     + ((seq_t)160264 * a3);
        a3 = a2;
        a2 = a1;
        a1 = curr;
    }
    return a1;
}
