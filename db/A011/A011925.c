/*
 * A011925  a(n) = floor(n*(n-1)*(n-2)*(n-3)/15).
 * https://oeis.org/A011925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/15)
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 505, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4U) return 0;
    unsigned __int128 prod =
        (unsigned __int128)n * (n - 1U) * (n - 2U) * (n - 3U);
    prod /= 15U;
    return (seq_t)prod;
}
