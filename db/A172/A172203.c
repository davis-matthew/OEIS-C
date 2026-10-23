/*
 * A172203  Number of ways to place 4 nonattacking kings on a 4 X n board.
 * https://oeis.org/A172203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64*n^4 - 720*n^3 + 3347*n^2 - 7569*n + 6894)/6 for n >=
 *            3, a(1) = 0, a(2) = 0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 143, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t num = (64 * n4) - (720 * n3) + (3347 * n2) - (7569 * nn) + 6894;
    return num / 6;
}
