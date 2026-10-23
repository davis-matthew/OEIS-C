/*
 * A141589  Frenicle's 6 X 6 square read by rows: contains numbers 1 to 24 and
 *          12 symmetric spaces (here represented by 0's).
 * https://oeis.org/A141589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   a(n) = entry n of Frenicle's 6×6 magic square read row‑wise,
 *            repeated cyclically (period 36). The square contains the numbers
 *            1..24 and 12 zeros such that every horizontal, vertical,
 *            principal diagonal and antidiagonal sum equals 50
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The 6×6 square in row‑major order */
    static const seq_t square[36] = {
        0,20, 1,24, 5, 0,
        21,17, 0, 0, 8, 4,
        16, 0,12,13, 0, 9,
        10, 0,18, 7, 0,15,
        3,11, 0, 0,14,22,
        0, 2,19, 6,23, 0
    };
    /* Periodic extension: a(n) = square[n mod 36] */
    return square[(size_t)(n % 36)];
}
