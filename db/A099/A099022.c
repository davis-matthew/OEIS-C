/*
 * A099022  a(n) = Sum_{k=0..n} C(n,k)*(2*n-k)!.
 * https://oeis.org/A099022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3 * Recurrence: a(n) = 2*n*(2*n-1)*a(n-1) +
 *            n*(n-1)*a(n-2) for n ≥ 2 * This implements OEIS sequence A099022
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 2*i*(2*i-1)*prev1 */
        seq_t term1 = prev1 * (seq_t)2 * (seq_t)i * (seq_t)(2*i - 1);
        /* term2 = i*(i-1)*prev2 */
        seq_t term2 = prev2 * (seq_t)i * (i - 1);
        prev2 = prev1;
        prev1 = term1 + term2;
    }
    return prev1;
}
