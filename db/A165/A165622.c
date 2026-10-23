/*
 * A165622  a(n)=(-4)*a(n-1)+8 with a(0)=1.
 * https://oeis.org/A165622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = (-4)*a(n-1)+8 */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) */
    if (n == 0)
        return val;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = ((seq_t)-4) * val + (seq_t)8;
    }
    return val;
}
