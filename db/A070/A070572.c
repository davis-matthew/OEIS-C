/*
 * A070572  a(n) = n^4 mod 29.
 * https://oeis.org/A070572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 29
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = (seq_t)29;
    seq_t x = (seq_t)n % mod;
    x = (x * x) % mod;          /* x = n^2 mod 29 */
    x = (x * x) % mod;          /* x = n^4 mod 29 */
    return x;
}
