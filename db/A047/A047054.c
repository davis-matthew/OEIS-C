/*
 * A047054  Duplicate of A052562.
 * https://oeis.org/A047054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n-1) * 5 * n, a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)5 * (seq_t)i;
    }
    return res;
}
