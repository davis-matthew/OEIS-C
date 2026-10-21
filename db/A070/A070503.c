/*
 * A070503  a(n) = n^3 mod 41.
 * https://oeis.org/A070503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 41
 * Range:     u8 n <= 285, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)41;
    seq_t m   = (seq_t)n % mod;   /* Reduce n modulo 41 */
    seq_t r   = m * m % mod;      /* m^2 mod 41 */
    r        = r * m % mod;      /* m^3 mod 41 */
    return r;
}
