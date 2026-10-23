/*
 * A155073  Expansion of (1 + 6*x)/(1 - 12*x - 8*x^2).
 * https://oeis.org/A155073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 8*a(n-2) with a(0)=1, a(1)=18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)18;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)18;  /* a(1) */
    seq_t curr   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)12 + prev2 * (seq_t)8;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
