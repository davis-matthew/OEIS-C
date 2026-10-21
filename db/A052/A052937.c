/*
 * A052937  Expansion of (2-3*x-x^2)/((1-x)*(1-2*x-x^2)).
 * https://oeis.org/A052937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2, with a(0)=2, a(1)=3, a(2)=6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    seq_t prev_prev = (seq_t)2;   /* a(0) */
    seq_t prev      = (seq_t)3;   /* a(1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)2 * prev + prev_prev - (seq_t)2;
        prev_prev = prev;
        prev      = curr;
    }
    return curr;
}
