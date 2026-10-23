/*
 * A159656  Numerator of Hermite(n, 18/19).
 * https://oeis.org/A159656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 36*a(n-1) - 722*(n-1)*a(n-2); Initial values: a(0) = 1,
 *            a(1) = 36
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)36;
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)36;  /* a(n-1) */
    seq_t cur;
    for (idx_t i = 2u; i <= n; ++i) {
        /* a(i) = 36*a(i-1) - 722*(i-1)*a(i-2) */
        cur = (seq_t)36 * prev1 - (seq_t)722 * ( (seq_t)(i - 1u) ) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
