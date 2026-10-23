/*
 * A166025  a(0) = 6, a(1) = 17, a(n+1) = a(n) + a(n-1) for n>0.
 * https://oeis.org/A166025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 6, a(1) = 17, a(n) = a(n-1) + a(n-2) for n > 1
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two defined initial terms explicitly. */
    if (n == (idx_t)0)
        return (seq_t)6;

    if (n == (idx_t)1)
        return (seq_t)17;

    /* Idle variables that hold the two most recent terms. */
    seq_t prev2 = (seq_t)6;   /* a(n-2) */
    seq_t prev1 = (seq_t)17;  /* a(n-1) */
    seq_t current;

    /* Iterate from the third term up to n. */
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        current = prev2 + prev1;  /* a(n) = a(n-1) + a(n-2) */
        prev2 = prev1;            /* shift window: a(n-1) becomes a(n-2) */
        prev1 = current;          /* a(n) becomes new a(n-1)     */
    }

    return prev1;   /* prev1 now holds a(n) */
}
