/*
 * A071245  a(n) = n*(n-1)*(2*n^2 + 1)/6.
 * https://oeis.org/A071245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(2*n^2 + 1) / 6
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 337, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow for large n. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * (nn - (unsigned __int128)1);
    prod *= (unsigned __int128)(2 * nn * nn + 1);
    prod /= 6;
    return (seq_t)prod;
}
