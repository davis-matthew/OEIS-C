/*
 * A163070  a(n) = ((4+sqrt(5))*(2+sqrt(5))^n + (4-sqrt(5))*(2-sqrt(5))^n)/2.
 * https://oeis.org/A163070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2), a(0) = 4, a(1) = 13
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)4;
    if (n == (idx_t)1) return (seq_t)13;

    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)13;  /* a(1) */
    seq_t current;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        current = prev1 * (seq_t)4 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
