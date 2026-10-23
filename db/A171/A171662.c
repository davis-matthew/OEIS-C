/*
 * A171662  a(n) = floor((2*n^2 + n)/6).
 * https://oeis.org/A171662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^2 + n)/6) = floor(n*(2*n + 1)/6)
 * Range:     u8 n <= 27, u16 n <= 443, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow when seq_t is 64‑bit. */
    unsigned __int128 num = (unsigned __int128)n * ((unsigned __int128)n * 2 + 1);
    return (seq_t)(num / 6);
}
