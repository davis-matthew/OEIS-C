/*
 * A158968  Numerator of Hermite(n, 1/6).
 * https://oeis.org/A158968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A158968: Numerator of Hermite(n, 1/6)
   Recurrence: a(0)=1, a(1)=1, a(n)=a(n-1)-18*(n-1)*a(n-2) for n>=2 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Iterative evaluation using only the last two terms. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = a(i-1) - 18*(i-1)*a(i-2) */
        a_cur = a_prev1
                - ((seq_t)18 * (seq_t)(i - 1) * a_prev2);
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_prev1;
}
