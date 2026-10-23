/*
 * A129266  Duplicate of A129527.
 * https://oeis.org/A129266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n if n is odd a(n) = a(n/2) + n if n is even (indices
 *            start at 1)
 * Range:     i8 n <= 10, i16 n <= 10, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: odd indices */
    if (n & 1)
        return (seq_t)n;

    /* Even indices: recursive definition */
    return a(n >> 1) + (seq_t)n;
}
