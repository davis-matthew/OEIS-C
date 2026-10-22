/*
 * A000545  Number of ways of n-coloring a dodecahedron.
 * https://oeis.org/A000545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 + 15*n^6 + 44*n^4) / 60
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n2 = n128 * n128;
    unsigned __int128 n4 = n2 * n2;
    unsigned __int128 n6 = n4 * n2;
    unsigned __int128 n12 = n6 * n6;
    
    return (seq_t)((n12 + 15 * n6 + 44 * n4) / 60);
}
