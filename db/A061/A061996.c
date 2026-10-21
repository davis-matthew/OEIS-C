/*
 * A061996  Number of ways to place 3 nonattacking kings on an n X n board.
 * https://oeis.org/A061996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*(n^4 + 3*n^3 - 20*n^2 - 30*n + 132)/6 (for
 *            n≥3) a(0) = a(1) = a(2) = 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;

    seq_t poly = N4                       // n^4
               + (seq_t)3 * N3            // + 3*n^3
               - (seq_t)20 * N2           // - 20*n^2
               - (seq_t)30 * N            // - 30*n
               + (seq_t)132;              // + 132

    seq_t factor = (N - (seq_t)1) * (N - (seq_t)2);

    return (factor * poly) / (seq_t)6;
}
