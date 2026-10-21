/*
 * A239072  Maximum number of cells in an n X n square grid that can be
 *          painted such that no two orthogonally adjacent cells are painted
 *          and every unpainted cell can be reached from the edge of the grid
 *          by a series of orthogonal steps to unpainted cells.
 * https://oeis.org/A239072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+2)/3) minus 1 if * (n mod 3 != 2) and (n+1 is
 *            not a power of 2); * otherwise a(n) = floor(n*(n+2)/3)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the upper bound: floor(n*(n+2)/3) */
    seq_t NN = (seq_t)n;
    seq_t upper = (NN * (NN + (seq_t)2)) / (seq_t)3;

    /* Determine whether to subtract 1 */
    bool subtract = false;
    if ((n % (idx_t)3) != (idx_t)2) {           /* n ≡ 0 or 1 (mod 3) */
        /* Check if n+1 is a power of two: (n+1) & n == 0 */
        if (!(((n + (idx_t)1) & n) == 0)) {
            subtract = true;
        }
    }

    return subtract ? upper - (seq_t)1 : upper;
}
