/*
 * A232007  Maximal number of moves needed by a knight to reach every square
 *          from a fixed position on an n X n chessboard, or -1 if it is not
 *          possible to reach every square.
 * https://oeis.org/A232007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=0; a(2)=a(3)=-1; a(4)=5; a(n)=ceil(2*n/3) for n>4
 * Range:     i8 n <= 146, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2 || n == 3) return -1;
    if (n == 4) return 5;
    /* For n > 4 */
    return ((seq_t)n * 2 + 2) / 3;
}
