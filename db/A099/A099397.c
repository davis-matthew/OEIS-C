/*
 * A099397  Chebyshev polynomial of the first kind, T(n,x), evaluated at x=51.
 * https://oeis.org/A099397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*a(n-1) - a(n-2), a(0)=1, a(1)=51
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)51;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)51;  /* a(1) */
    seq_t cur;

    /* Iterate the recurrence up to n */
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)102 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
