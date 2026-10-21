/*
 * A083858  Expansion of x/(1 - 3*x - 6*x^2).
 * https://oeis.org/A083858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Iterate the recurrence from 2 up to n. */
    seq_t prev2 = (seq_t)0;  /* a(n-2) */
    seq_t prev1 = (seq_t)1;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)3 + prev2 * (seq_t)6;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
