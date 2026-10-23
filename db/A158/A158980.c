/*
 * A158980  Numerator of Hermite(n, 1/7).
 * https://oeis.org/A158980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7^n * Hermite(n,1/7) Recurrence: a(0) = 1, a(1) = 2, a(n)
 *            = 2*a(n-1) - 98*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev  = (seq_t)2;          /* a(1) */
    seq_t term, subtract;
    for (idx_t i = 2; i <= n; ++i) {
        term = a_prev * (seq_t)2;
        subtract = a_prev2 * (seq_t)98 * (seq_t)(i - 1);
        term -= subtract;
        a_prev2 = a_prev;
        a_prev = term;
    }
    return a_prev;
}
