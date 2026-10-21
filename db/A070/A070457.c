/*
 * A070457  a(n) = n^2 mod 35.
 * https://oeis.org/A070457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 35
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce n modulo 35 first to avoid overflow in intermediate multiplication */
    seq_t n_mod = (seq_t)(n % 35ULL);
    return (n_mod * n_mod) % (seq_t)35;
}
