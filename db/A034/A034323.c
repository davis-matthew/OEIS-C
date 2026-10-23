/*
 * A034323  a(n) = n-th quintic factorial number divided by 2.
 * https://oeis.org/A034323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{j=1..n} (5*j-3)/2 Recurrence: a(1)=1; for n>1, a(n) =
 *            (5*n-3) * a(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard */
    seq_t result = 1;               /* start with 1; factor 2 is canceled later */
    /* skip j = 1 since (5*1-3)=2 and division by 2 cancels it */
    for (idx_t j = 2; j <= n; ++j)
    {
        seq_t mult = (seq_t)5 * j - 3;
        result *= mult;
    }
    return result;
}
