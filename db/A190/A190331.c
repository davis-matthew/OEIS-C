/*
 * A190331  a(n) = 8*a(n-1) + 2*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 2*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2)  */
    seq_t prev1 = (seq_t)1;   /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)8 + prev2 * (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
