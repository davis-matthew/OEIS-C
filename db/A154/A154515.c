/*
 * A154515  a(n) = 648*n^2 + 72*n + 1.
 * https://oeis.org/A154515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 648*n^2 + 72*n + 1 (OFFSET 1)
 * Range:     u8 none, u16 n <= 10, u32 n <= 2574, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but safe for calls with n < 1 */
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)648 * nn * nn + (seq_t)72 * nn + 1;
    return res;
}
