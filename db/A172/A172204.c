/*
 * A172204  Number of ways to place 5 nonattacking kings on a 5 X n board.
 * https://oeis.org/A172204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (625*n^5 - 9750*n^4 + 66415*n^3 - 247626*n^2 + 504664*n -
 *            446544)/24 for n>=4; a(1)=0, a(2)=0, a(3)=15
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 15;

    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;

    seq_t numer = (seq_t)625 * x5;
    numer -= (seq_t)9750 * x4;
    numer += (seq_t)66415 * x3;
    numer -= (seq_t)247626 * x2;
    numer += (seq_t)504664 * x;
    numer -= (seq_t)446544;

    return numer / (seq_t)24;
}
