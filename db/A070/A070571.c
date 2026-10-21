/*
 * A070571  a(n) = n^4 mod 28.
 * https://oeis.org/A070571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 28
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = 28;
    seq_t x   = (seq_t)n % mod;      /* n modulo 28 */
    seq_t res = x;                   /* x^1 */
    res = (res * res) % mod;         /* x^2 */
    res = (res * x)   % mod;         /* x^3 */
    res = (res * x)   % mod;         /* x^4 */
    return res;
}
