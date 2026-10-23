/*
 * A147725  Row sums of triangle in A147724.
 * https://oeis.org/A147725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 11*a(n-2); a(0)=1, a(1)=2, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)10;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)9 * prev1 - (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
