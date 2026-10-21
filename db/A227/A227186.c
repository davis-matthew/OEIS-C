/*
 * A227186  Array A(n,k) read by antidiagonals: the length of the (k+1)-th run
 *          (k>=0) of binary digits of n, first run starting from the least
 *          significant bit of n.
 * https://oeis.org/A227186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) returns the length of the (k+1)-th run of binary digits *
 *            of n, where (n,k) is obtained from n by the antidiagonal
 *            mapping. * Runs start at the least significant bit. If there are
 *            fewer than * k+1 runs, the result is zero. The mapping is
 *            performed by * antidiagonal_coords(n,&row,&col). See O...
 * Range:     u8 n <= 9175, u16 n <= 9175, u32 n <= 9175, u64 n <= 9175,
 *            u128 n <= 9175, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);   /* map flattened index to (row,col) */

    if (row == 0)                     /* row 0 has no runs by convention */
        return (seq_t)0;

    idx_t x = row;
    idx_t run = 0;                    /* zero-based run counter */

    while (x > 0) {
        idx_t current = x & 1;        /* current bit */
        idx_t len = 1;                /* current run length starts at 1 */

        x >>= 1;                      /* move to next bit */

        /* extend current run while next bit equals current bit */
        while (x > 0 && (x & 1) == current) {
            len++;
            x >>= 1;
        }

        if (run == col)               /* found the desired run */
            return (seq_t)len;

        run++;                        /* move to next run */
    }

    /* fewer runs than requested */
    return (seq_t)0;
}
