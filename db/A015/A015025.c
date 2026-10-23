/*
 * A015025  q-factorial numbers for q=-10.
 * https://oeis.org/A015025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=1..n} ((-10)^k - 1) / (-10 - 1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = ((-10)^n - 1) * a(n-1) / (-11) */

seq_t a(idx_t n)
{
    /* constants */
    const seq_t negten  = (seq_t)-10;
    const seq_t divider = (seq_t)-11;
    /* intermediate values */
    seq_t power  = (seq_t)1;   /* holds (-10)^i   */
    seq_t result = (seq_t)1;   /* Holds a(i-1)    */

    for (idx_t i = 1; i <= n; ++i) {
        power  = power * negten;          /* (-10)^i  */
        seq_t term = power - (seq_t)1;    /* (-10)^i - 1 */
        result = (result * term) / divider;  /* / (-11) */
    }
    return result;
}
