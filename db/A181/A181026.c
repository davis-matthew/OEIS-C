/*
 * A181026  Maximum number of 1s in an nX(n+2) binary matrix with no four 1s
 *          adjacent in a line along a row, column or diagonally.
 * https://oeis.org/A181026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ceil(3 * n * (n + 2) / 4) - 2 * floor((n - 3) / 2)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset start at n = 1 */

    /* use a wider type for intermediate calculations to avoid overflow */
    idx_t prod = (idx_t)n * ((idx_t)n + 2);        /* n * (n + 2) */
    idx_t ceil_val = (3 * prod + 3) / 4;          /* ceil(3*n*(n+2)/4) */
    idx_t sub = 0;
    if (n >= 3)
        sub = 2 * ((n - 3) / 2);                 /* 2 * floor((n - 3)/2) */

    return (seq_t)(ceil_val - sub);
}
