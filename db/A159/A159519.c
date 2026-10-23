/*
 * A159519  Numerator of Hermite(n, 13/15).
 * https://oeis.org/A159519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 26*a(n-1) - 450*(n-1)*a(n-2), a(0) = 1, a(1) = 26
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)26;

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)26;         /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* curr = 26*prev1 - 450*(i-1)*prev2 */
        curr = (seq_t)26 * prev1
             - (seq_t)450 * (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
