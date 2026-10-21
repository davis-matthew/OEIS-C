/*
 * A099926  Duplicate of A098600.
 * https://oeis.org/A099926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n-1) + a(n-2) - (-1)^n, a(0)=1, a(1)=2
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    seq_t prev1 = 1;    /* a(0) */
    seq_t prev2 = 2;    /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sign = (i & 1) ? -1 : 1;   /* (-1)^i */
        seq_t cur = prev1 + prev2 - sign;
        prev1 = prev2;
        prev2 = cur;
    }
    return prev2;
}
