/*
 * A165625  a(n)=(5/3)*(1+2*(-5)^(n-1)).
 * https://oeis.org/A165625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (5/3)*(1 + 2*(-5)^(n-1))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = (-5)*a(n-1) + 10 for n ≥ 1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* iterate from a(1) to a(n) */
    {
        val = (seq_t)(-5) * val + (seq_t)10;
    }
    return val;
}
