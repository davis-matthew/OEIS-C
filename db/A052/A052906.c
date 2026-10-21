/*
 * A052906  Expansion of (1-x^2)/(1-3*x-x^2).
 * https://oeis.org/A052906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=9, for n>=3: a(n)=3*a(n-1)+a(n-2)
 *            (recurrence derived from the generating function)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Iteratively compute from a(1)=3 and a(2)=9 */
    seq_t prev2 = (seq_t)3;  /* a(n-2) when n=3 */
    seq_t prev1 = (seq_t)9;  /* a(n-1) when n=3 */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = 3 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
