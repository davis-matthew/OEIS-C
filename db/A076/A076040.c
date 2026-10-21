/*
 * A076040  a(n) = (-1)^n * (3^n - 1)/2.
 * https://oeis.org/A076040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * (3^n - 1)/2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=-1, a(n) = -4*a(n-1) - 3*a(n-2) for n>=2 */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;

    seq_t prev2 = (seq_t)0;          /* a(0) */
    seq_t prev1 = (seq_t)-1;         /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp1 = (seq_t)4 * prev1;      /* 4 * a(n-1) */
        seq_t temp2 = (seq_t)3 * prev2;      /* 3 * a(n-2) */
        curr = -temp1 - temp2;               /* -4*a(n-1) - 3*a(n-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
