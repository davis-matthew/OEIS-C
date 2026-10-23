/*
 * A008738  a(n) = floor((n^2 + 1)/5).
 * https://oeis.org/A008738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 1) / 5)
 * Range:     u8 n <= 35, u16 n <= 572, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a wider type for the intermediate product to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)n + 1;
    return (seq_t)(prod / 5);
}
