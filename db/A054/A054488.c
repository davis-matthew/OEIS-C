/*
 * A054488  Expansion of (1+2*x)/(1-6*x+x^2).
 * https://oeis.org/A054488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 8, a(n) = 6*a(n-1) - a(n-2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(n-2)  */
    seq_t prev1 = (seq_t)8;   /* a(n-1)  */
    seq_t curr;               /* a(n)    */

    for (idx_t i = 2; i <= n; ++i) {
        curr = 6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
