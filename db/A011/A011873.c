/*
 * A011873  a(n) = floor(n(n-1)/20).
 * https://oeis.org/A011873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (n-1) / 20)
 * Range:     u8 n <= 72, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)(n-1);
    prod /= 20;
    return (seq_t)prod;
}
