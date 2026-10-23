/*
 * A165813  a(n) = n*(a(n-1)+3), a(0)=1.
 * https://oeis.org/A165813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(a(n-1)+3), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;               /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        res = (seq_t)i * (res + (seq_t)3); /* a(i) = i*(a(i-1)+3) */
    }
    return res;
}
