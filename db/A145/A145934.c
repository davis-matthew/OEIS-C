/*
 * A145934  Expansion of 1/(1-x*(1-6*x)).
 * https://oeis.org/A145934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 6*a(n-2) (a(0)=1, a(1)=1)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 24, i64 n <= 49, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 - (seq_t)6 * a0; /* a(i) = a(i-1) - 6*a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a2;
}
