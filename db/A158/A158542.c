/*
 * A158542  a(n) = Hermite(n,13).
 * https://oeis.org/A158542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=26, a(n)=26*a(n-1)-2*(n-1)*a(n-2) */
/* This implements Hermite(n,13) as defined in OEIS A158542 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)26;
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)26;
    seq_t cur = (seq_t)0;
    for (idx_t i = 2u; i <= n; ++i)
    {
        cur = (seq_t)26 * prev1 - (seq_t)2 * (seq_t)(i - 1u) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
