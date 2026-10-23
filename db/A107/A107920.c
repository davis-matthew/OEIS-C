/*
 * A107920  Lucas and Lehmer numbers with parameters (1 +- sqrt(-7))/2.
 * https://oeis.org/A107920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 16, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0; /* a(0) */
    seq_t prev1 = 1; /* a(1) */
    seq_t cur;

    /* Iterate from 2 up to n inclusive */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur   = prev1 - (2 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
