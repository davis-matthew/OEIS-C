/*
 * A154245  a(n) = ( (4 + sqrt(7))^n - (4 - sqrt(7))^n )/(2*sqrt(7)).
 * https://oeis.org/A154245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 9*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1U) return 1U;          /* a(1) = 1  */
    if (n == 2U) return 8U;          /* a(2) = 8  */

    /* Iteratively apply the recurrence */
    seq_t prev2 = 1U;                /* a(1) */
    seq_t prev1 = 8U;                /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t curr = prev1 * 8ULL - prev2 * 9ULL;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
