/*
 * A009117  Expansion of e.g.f.: 1/2 + exp(-4*x)/2.
 * https://oeis.org/A009117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = (-4)^n / 2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* compute (-4)^n */
    for (idx_t i = 0; i < n; ++i)
    {
        result *= (seq_t)-4;
    }
    /* divide by 2 */
    result /= (seq_t)2;
    return result;
}
