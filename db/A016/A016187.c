/*
 * A016187  Expansion of 1/((1-8*x)*(1-11*x)).
 * https://oeis.org/A016187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^(n+1) - 8^(n+1))/3 * Recurrence: a(0) = 1, a(1) = 19,
 *            * a(n) = 19*a(n-1) - 88*a(n-2) (for n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)19;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)19 * prev1) - ((seq_t)88 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
