/*
 * A087291  Number of pairs of polynomials (f,g) in GF(2)[x] satisfying 1 <=
 *          deg(f) <= n, 1 <= deg(g) <= n and gcd(f,g) = 1.
 * https://oeis.org/A087291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2^n - 1)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t two_pow = 1;
    for (idx_t i = 0; i < n; ++i)
        two_pow <<= 1;          /* two_pow = 2^n */
    two_pow -= 1;               /* two_pow = 2^n - 1 */
    two_pow *= two_pow;         /* two_pow = (2^n - 1)^2 */
    two_pow <<= 1;              /* two_pow = 2*(2^n - 1)^2 */
    return two_pow;
}
