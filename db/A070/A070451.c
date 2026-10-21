/*
 * A070451  a(n) = n^2 mod 29.
 * https://oeis.org/A070451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 29
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 29 first to avoid any overflow. */
    idx_t r = n % 29ULL;        /* r is in [0,28] */
    return (seq_t)((r * r) % 29ULL);
}
