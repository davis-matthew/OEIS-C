/*
 * A086219  Duplicate of A055010.
 * https://oeis.org/A086219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + 1 with a(0) = 0, a(1) = 2
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;         /* a(0) */
    seq_t res = 2;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        res = res * 2 + 1;   /* recurrence */
    return res;
}
