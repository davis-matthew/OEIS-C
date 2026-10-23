/*
 * A158751  a(n) = Hermite(n, 22).
 * https://oeis.org/A158751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 44*a(n-1) - 2*(n-1)*a(n-2); a(0)=1, a(1)=44
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)44;

    /* Iterate from 2 up to n using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)44;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* cur = 44*prev1 - 2*(i-1)*prev2 */
        cur = (seq_t)44 * prev1
              - (seq_t)2 * ((seq_t)(i - 1)) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
