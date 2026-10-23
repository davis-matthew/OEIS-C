/*
 * A018836  Number of squares on infinite chessboard at <= n knight's moves
 *          from a fixed square.
 * https://oeis.org/A018836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1-6*n+14*n^2+4*sign(n*(n-1)*(n-3))
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = 14 * N * N;
    term -= 6 * N;
    term += 1;
    if (n >= 4) {
        term += 4;
    } else if (n == 2) {
        term -= 4;
    }
    return term;
}
