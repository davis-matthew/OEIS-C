/*
 * A174452  a(n) = n^2 mod 1000.
 * https://oeis.org/A174452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = ((n % 1000) * (n % 1000)) % 1000
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 1000;          /* r ∈ [0,999] */
    return (seq_t)((r * r) % 1000);
}
