/*
 * A070442  a(n) = n^2 mod 20.
 * https://oeis.org/A070442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2) mod 20
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 20 first to avoid overflow */
    seq_t r = (seq_t)(n % 20ULL);
    /* Compute the square modulo 20 */
    return (r * r) % (seq_t)20;
}
