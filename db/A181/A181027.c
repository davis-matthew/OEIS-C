/*
 * A181027  Maximum number of 1s in an nX(n+3) binary matrix with no four 1s
 *          adjacent in a line along a row, column or diagonally.
 * https://oeis.org/A181027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(n+2) for n <= 3 a(n) = n^2 - n + 8 for n >= 4
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 3u) {
        return (seq_t)n * ((seq_t)n + 2u);
    } else {
        seq_t nn = (seq_t)n * (seq_t)n;
        return nn - (seq_t)n + (seq_t)8;
    }
}
