/*
 * A198647  a(n) = 3*7^n-1.
 * https://oeis.org/A198647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3*7^n - 1 (a(0)=2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 2;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * 7 + 6;  /* a(i) = 7*a(i-1) + 6 */
    }
    return res;
}
