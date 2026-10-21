/*
 * A070469  a(n) = n^2 mod 47.
 * https://oeis.org/A070469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 47
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce n modulo 47 first to avoid overflow in seq_t */
    idx_t r = n % 47;
    seq_t x = (seq_t)r;
    return (seq_t)((x * x) % 47);
}
