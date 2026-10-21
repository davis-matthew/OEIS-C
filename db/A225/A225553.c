/*
 * A225553  Longest checkmate in king and amazon versus king endgame on an n X
 *          n chessboard.
 * https://oeis.org/A225553
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2)/2) for n>=11; a(3)=0, a(4)=1, a(5)=2, a(6)=3,
 *            a(7)=4, a(8)=4, a(9)=5, a(10)=5
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (n == 3) return 0;
    if (n == 4) return 1;
    if (n == 5) return 2;
    if (n == 6) return 3;
    if (n == 7) return 4;
    if (n == 8) return 4;
    if (n == 9) return 5;
    if (n == 10) return 5;
    return ((seq_t)n + 2) / 2;
}
