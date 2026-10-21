/*
 * A070434  a(n) = n^2 mod 11.
 * https://oeis.org/A070434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 11 first to avoid overflow and keep the value small. */
    seq_t r = (seq_t)(n % 11ULL);
    return (r * r) % 11;
}
