/*
 * A215774  Number of undirected labeled graphs on n+4 nodes with exactly n
 *          cycle graphs as connected components.
 * https://oeis.org/A215774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(15*n^3+30*n^2+245*n+286)/5760
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 537, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 prod_five = N * (N + 1) * (N + 2) * (N + 3) * (N + 4);
    unsigned __int128 poly = (15 * N * N * N + 30 * N * N + 245 * N + 286);
    unsigned __int128 numerator = prod_five * poly;
    numerator /= 5760;
    return (seq_t)numerator;
}
