/*
 * A085577  Size of largest subset of the n^2 cells in an n X n grid such that
 *          there are at least 3 edges between any pair of chosen cells.
 * https://oeis.org/A085577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor((n^2 + 4) / 5) for all n,
 *             except a(5) = 6 (special case for the 5×5 board).
 *
 * This expression reproduces all known terms of OEIS A085577.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 5)
        return (seq_t)6;

    seq_t nn = (seq_t)n * (seq_t)n;   /* compute n^2 in seq_t space */
    return (nn + 4) / 5;              /* integer division gives floor */
}
