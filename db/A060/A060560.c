/*
 * A060560  Number of ways to color the vertices of an octagon using <= n
 *          colors, allowing rotations and reflections.
 * https://oeis.org/A060560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^6 - n^5 + n^4 + 3*n^3 + 2*n^2 - 2*n + 4)/16
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 362,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned __int128 N = n;

    unsigned __int128 poly = N * N * N * N * N * N;      // n^6
    poly -= N * N * N * N * N;                         // - n^5
    poly += N * N * N * N;                             // + n^4
    poly += 3 * N * N * N;                             // + 3 n^3
    poly += 2 * N * N;                                 // + 2 n^2
    poly -= 2 * N;                                     // - 2 n
    poly += 4;                                         // + 4

    unsigned __int128 val = N * (N + 1);
    val = val * poly;
    val = val / 16;

    return (seq_t)val;
}
