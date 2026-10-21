/*
 * A090313  a(n) = 22*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 22.
 * https://oeis.org/A090313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 22*a(n-1) + a(n-2), a(0)=2, a(1)=22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)22;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)22;  /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)22 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
