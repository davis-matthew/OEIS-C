/*
 * A053537  Expansion of 1/((1+5*x)*(1-15*x)).
 * https://oeis.org/A053537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + 75*a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t current;

    /* Iteratively compute a(n) using the recurrence */
    for (idx_t i = 2u; i <= n; ++i)
    {
        current = (seq_t)10 * prev1 + (seq_t)75 * prev2;
        prev2   = prev1;
        prev1   = current;
    }

    return prev1;
}
