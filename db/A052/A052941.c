/*
 * A052941  Expansion of (1-x)/(1 - 4*x + x^2 + x^3).
 * https://oeis.org/A052941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - a(n-3), with a(0)=1, a(1)=3, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;

    /* Sliding window of the last three values */
    seq_t prev3 = (seq_t)1;   /* a(n-3) */
    seq_t prev2 = (seq_t)3;   /* a(n-2) */
    seq_t prev1 = (seq_t)11;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute a(i) = 4*a(i-1) - a(i-2) - a(i-3) */
        cur = prev1 * (seq_t)4;
        cur -= prev2;
        cur -= prev3;

        /* Roll the window forward */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
