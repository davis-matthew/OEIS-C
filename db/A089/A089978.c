/*
 * A089978  Expansion of 1/(1-3x-3x^3).
 * https://oeis.org/A089978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-3) Initial values: a(0)=1, a(1)=3,
 *            a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;
    if (n == 2U) return (seq_t)9;

    seq_t prev3 = (seq_t)1; /* a(0) */
    seq_t prev2 = (seq_t)3; /* a(1) */
    seq_t prev1 = (seq_t)9; /* a(2) */
    seq_t curr   = 0;

    for (idx_t i = 3U; i <= n; ++i)
    {
        curr = 3 * prev1 + 3 * prev3; /* a(i) = 3*a(i-1)+3*a(i-3) */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
