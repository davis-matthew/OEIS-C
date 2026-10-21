/*
 * A225552  Longest checkmate in king and rook versus king endgame on an n X n
 *          chessboard.
 * https://oeis.org/A225552
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 3 + floor((n+1)/2) - floor(n/6) for n > 24. For 3 ≤
 *            n ≤ 24 the values are taken from a pre‑computed table
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed values for 3 ≤ n ≤ 24. */
    static const seq_t tbl[25] = {
        0,  /* 0  unused   */
        0,  /* 1  unused   */
        0,  /* 2  unused   */
        3,  /* a(3)  */
        7,  /* a(4)  */
        10, /* a(5)  */
        12, /* a(6)  */
        14, /* a(7)  */
        16, /* a(8)  */
        18, /* a(9)  */
        21, /* a(10) */
        23, /* a(11) */
        25, /* a(12) */
        28, /* a(13) */
        30, /* a(14) */
        33, /* a(15) */
        35, /* a(16) */
        38, /* a(17) */
        39, /* a(18) */
        42, /* a(19) */
        44, /* a(20) */
        47, /* a(21) */
        49, /* a(22) */
        52, /* a(23) */
        54  /* a(24) */
    };

    if (n < 3)
        return (seq_t)0;

    if (n <= 24U)
        return tbl[(size_t)n];

    /* Closed form for n > 24. */
    seq_t nn = (seq_t)n;
    return 2 * nn - 3 + (nn + 1) / 2 - nn / 6;
}
