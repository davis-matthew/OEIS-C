/*
 * A101624  Stern-Jacobsthal numbers.
 * https://oeis.org/A101624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) XOR (a(n-2) * 2); a(0) = 1, a(1) = 1
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)1;   /* a(n-2)  */
    seq_t a1 = (seq_t)1;   /* a(n-1)  */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = a1 ^ (a0 << 1);  /* a(n) = a(n-1) XOR (a(n-2)*2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
