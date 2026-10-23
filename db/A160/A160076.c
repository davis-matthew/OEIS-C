/*
 * A160076  Numerator of Hermite(n, 17/26).
 * https://oeis.org/A160076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=17, a(n)=17*a(n-1)-338*(n-1)*a(n-2) for n>=2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)17;     /* a(1) */
    seq_t curr;
    for (idx_t i = 1; i < n; ++i) {
        /* compute a(i+1) = 17*a(i) - 338*i*a(i-1) */
        idx_t coef = i;          /* (n-1) in the recurrence */
        seq_t term  = (seq_t)338 * (seq_t)coef * prev2;
        curr = (seq_t)17 * prev1 - term;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
