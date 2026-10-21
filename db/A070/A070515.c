/*
 * A070515  a(n) = n^4 mod 11.
 * https://oeis.org/A070515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 11 first – the result is < 11 */
    idx_t r   = n % 11ULL;
    idx_t r2  = (r * r) % 11ULL;      /* r^2 mod 11 */
    idx_t r4  = (r2 * r2) % 11ULL;    /* r^4 mod 11 */
    return (seq_t)r4;
}
