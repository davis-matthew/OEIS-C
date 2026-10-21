/*
 * A087619  a(n) = 137*a(n-1) + a(n-2), with a(0) = 2 and a(1) = 137.
 * https://oeis.org/A087619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 137*a(n-1) + a(n-2), a(0) = 2, a(1) = 137
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)137;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)137; /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 137 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
