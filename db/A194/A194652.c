/*
 * A194652  Number of ways to place 4 nonattacking kings on an n X n
 *          cylindrical chessboard.
 * https://oeis.org/A194652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/24*n*(n^7 - 54*n^5 + 36*n^4 + 1019*n^3 - 1236*n^2 -
 *            6690*n + 10884), n>=5 a(1)=0, a(2)=0, a(3)=0, a(4)=32
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;
    if (n == 4) return (seq_t)32;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    seq_t n5  = n4 * N;
    seq_t n7  = n5 * n2;  /* n^7 = n^5 * n^2 */

    seq_t term =
        n7
        - (seq_t)54 * n5
        + (seq_t)36 * n4
        + (seq_t)1019 * n3
        - (seq_t)1236 * n2
        - (seq_t)6690 * N
        + (seq_t)10884;

    seq_t numerator = N * term;
    return numerator / (seq_t)24;   /* exact division */
}
