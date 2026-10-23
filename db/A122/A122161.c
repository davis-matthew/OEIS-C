/*
 * A122161  Expansion of x*(1 - 3*x + x^2) / (1 - x - 2*x^2 + x^3).
 * https://oeis.org/A122161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) for n >= 4 a(1) = 1, a(2) =
 *            -2, a(3) = 1
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)-2;
        return (seq_t)1;
    }
    seq_t a1 = (seq_t)1;      /* a(1) */
    seq_t a2 = (seq_t)-2;     /* a(2) */
    seq_t a3 = (seq_t)1;      /* a(3) */
    seq_t res;

    for (idx_t i = 4; i <= n; ++i) {
        res = a3 + (seq_t)2 * a2 - a1;  /* recurrence */
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return a3;
}
