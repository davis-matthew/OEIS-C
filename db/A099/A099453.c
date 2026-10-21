/*
 * A099453  Expansion of 1/(1 - 7*x + 11*x^2).
 * https://oeis.org/A099453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=7; a(n)=7*a(n-1)-11*a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = 1;
    seq_t prev1 = 7;
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 7 * prev1 - 11 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
