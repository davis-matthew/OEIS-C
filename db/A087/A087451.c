/*
 * A087451  G.f.: (2-x)/((1+2*x)*(1-3*x)); e.g.f.: exp(3*x) + exp(-2*x); a(n)
 *          = 3^n + (-2)^n.
 * https://oeis.org/A087451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + (-2)^n Recurrence: a(0) = 2, a(1) = 1, a(n) =
 *            a(n-1) + 6*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)2;
    if (n == (idx_t)1) return (seq_t)1;

    seq_t prev2 = (seq_t)2; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        cur   = (seq_t)((seq_t)6 * prev2 + prev1);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
