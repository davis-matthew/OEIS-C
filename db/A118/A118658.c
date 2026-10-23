/*
 * A118658  a(n) = 2*F(n-1) = L(n) - F(n), where F(n) and L(n) are Fibonacci
 *          and Lucas numbers respectively.
 * https://oeis.org/A118658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2); a(0)=2, a(1)=0
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)0;

    seq_t prev2 = (seq_t)2; /* a(n-2) */
    seq_t prev1 = (seq_t)0; /* a(n-1) */
    seq_t curr = prev1 + prev2;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
