/*
 * A172127  Number of ways to place 4 nonattacking bishops on an n X n board.
 * https://oeis.org/A172127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n(n−2)(15n⁶−90n⁵+260n⁴−524n³+727n²−646n+348)/360 if n is
 *            even (n−1)(n−2)(15n⁶−75n⁵+185n⁴−339n³+388n²−258n+180)/360 if n
 *            is odd
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 381, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to keep intermediate values exact */
    unsigned __int128 N  = n;
    unsigned __int128 N2 = N * N;
    unsigned __int128 N3 = N2 * N;
    unsigned __int128 N4 = N3 * N;
    unsigned __int128 N5 = N4 * N;
    unsigned __int128 N6 = N5 * N;
    unsigned __int128 poly;
    unsigned __int128 num;
    if ((n & 1ULL) == 0) {                /* n is even */
        poly = (unsigned __int128)15 * N6
             - (unsigned __int128)90 * N5
             + (unsigned __int128)260 * N4
             - (unsigned __int128)524 * N3
             + (unsigned __int128)727 * N2
             - (unsigned __int128)646 * N
             + 348;
        num = N * (N - 2) * poly;
    } else {                                /* n is odd */
        poly = (unsigned __int128)15 * N6
             - (unsigned __int128)75 * N5
             + (unsigned __int128)185 * N4
             - (unsigned __int128)339 * N3
             + (unsigned __int128)388 * N2
             - (unsigned __int128)258 * N
             + 180;
        num = (N - 1) * (N - 2) * poly;
    }
    return (seq_t)(num / 360);
}
