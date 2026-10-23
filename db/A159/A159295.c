/*
 * A159295  Number of ways that a tile in the form of a strip of n congruent
 *          regular hexagons stuck together on successive parallel edges can
 *          be surrounded by one layer of copies of itself in a plane. Ways
 *          that differ by rotation or reflection are not counted as
 *          different. The surrounded tile is the exact su...
 * https://oeis.org/A159295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/144)*(n^6 + 30*n^5 + 463*n^4 + 3132*n^3 + 11506*n^2 +
 *            10716*n - 1152 + (n odd)*(9*n^2 + 90*n + 261)) for n>2, a(1)=1,
 *            a(2)=747
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 87, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)747;

    seq_t NN = (seq_t)n;
    unsigned __int128 n128 = (unsigned __int128)NN;
    unsigned __int128 n2 = n128 * n128;          /* n^2   */
    unsigned __int128 n3 = n2 * n128;            /* n^3   */
    unsigned __int128 n4 = n3 * n128;            /* n^4   */
    unsigned __int128 n5 = n4 * n128;            /* n^5   */
    unsigned __int128 n6 = n5 * n128;            /* n^6   */

    unsigned __int128 sum =
          n6
        + (unsigned __int128)30 * n5
        + (unsigned __int128)463 * n4
        + (unsigned __int128)3132 * n3
        + (unsigned __int128)11506 * n2
        + (unsigned __int128)10716 * n128
        - 1152;

    if ((n & 1) != 0)   /* n odd */
        sum += (unsigned __int128)9 * n2
             + (unsigned __int128)90 * n128
             + 261;

    return (seq_t)(sum / 144);
}
