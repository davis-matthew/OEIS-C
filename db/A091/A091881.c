/*
 * A091881  Expansion of (1-11*x)/((1-x)*(1-16*x)).
 * https://oeis.org/A091881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16^n/3 + 2/3 Recurrence: a(0) = 1, a(1) = 6, a(n) =
 *            17*a(n-1) - 16*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n cases: directly return the known values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Use the linear recurrence to generate the terms up to n.
       Only two previous terms are required, so the memory usage is constant. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 17 * prev1 - 16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
