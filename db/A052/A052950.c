/*
 * A052950  Expansion of (2-3*x-x^2+x^3)/((1-x)*(1+x)*(1-2*x)).
 * https://oeis.org/A052950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + 1 + (-1)^n + 0^n)/2 Recurrence: a(0)=2, a(1)=1,
 *            a(2)=3, a(n)=a(n-1)+2*a(n-2)-1 for n>=3 Implements the OEIS
 *            sequence A052950
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(n-2), starts with a(1) */
    seq_t prev1 = (seq_t)3;   /* a(n-1), starts with a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 + (prev2 << 1) - (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
