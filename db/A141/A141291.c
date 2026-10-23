/*
 * A141291  a(n) = 4*a(n-1) + 2*n - 1.
 * https://oeis.org/A141291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*n - 1, with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t res = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        res = res * 4 + (seq_t)(((seq_t)i) * 2 - 1);
    }
    return res;
}
