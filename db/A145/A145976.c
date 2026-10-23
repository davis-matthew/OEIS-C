/*
 * A145976  Expansion of 1/(1-x*(1-7*x)).
 * https://oeis.org/A145976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 7*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 22, i64 n <= 45, i128 n <= 90,
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
        return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 - (seq_t)7 * prev2;  /* a(i) = a(i-1) - 7*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
