/*
 * A159968  Numerator of Hermite(n, 11/24).
 * https://oeis.org/A159968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 12^n * Hermite(n, 11/24) Equivalent recurrence: a(0) = 1
 *            a(1) = 11 a(n) = 11 * a(n-1) - 288 * (n-1) * a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a_prev_prev = (seq_t)1;   /* a(0) */
    seq_t a_prev      = (seq_t)11;  /* a(1) */
    seq_t a_curr = a_prev;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 11*a(i-1) - 288*(i-1)*a(i-2) */
        seq_t term1 = (seq_t)11 * a_prev;
        seq_t term2 = (seq_t)288 * (i - 1);
        a_curr = term1 - term2 * a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }

    return a_curr;
}
