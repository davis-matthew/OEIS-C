/*
 * A070587  a(n) = n^4 mod 44.
 * https://oeis.org/A070587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 44
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = 44;
    seq_t r = (seq_t)n % mod;     /* reduce n first to avoid large intermediates */
    seq_t r2 = (r * r) % mod;      /* r^2 mod 44 */
    seq_t r4 = (r2 * r2) % mod;    /* r^4 mod 44 */
    return r4;
}
