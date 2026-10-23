/*
 * A101386  Expansion of g.f.: (5 - 3*x)/(1 - 6*x + x^2).
 * https://oeis.org/A101386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), with a(0) = 5, a(1) = 27
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)5;
    if (n == 1u) return (seq_t)27;
    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)27;  /* a(1) */
    seq_t curr  = prev1;      /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
