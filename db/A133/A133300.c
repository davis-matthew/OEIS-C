/*
 * A133300  Square array read along upward antidiagonals. S(n,m) is the number
 *          of domino tilings of an n-row and m-column checkerboard with a
 *          black upper-left square, where any vertical dominoes are allowed
 *          and horizontal dominoes must be placed so that the black square is
 *          on the left.
 * https://oeis.org/A133300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the number of domino tilings of an i-row, j-column board
 *            with a black upper-left square, where i and j are read along
 *            upward antidiagonals: S(i,j) = 0 if i and j are odd, = 1 if i is
 *            even, = ((i+1)/2)^(j/2) if i is odd and j is even
 * Range:     u8 n <= 98, u16 n <= 244, u32 n <= 651, u64 n <= 1688,
 *            u128 n <= 4434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t d = 2;          /* current diagonal index (sum i+j)     */
    idx_t cum = 0;        /* cumulative positions before this diag */

    /* Find the diagonal that contains n. */
    while (cum + (d - 1) < n) {
        cum += d - 1;
        d++;
    }

    /* Position within the diagonal. offset is 1 .. (d-1). */
    idx_t offset = n - cum;
    idx_t i = d - offset; /* row index   */
    idx_t j = offset;     /* column index */

    if ((i & 1) == 0) {
        return (seq_t)1; /* even i -> 1 */
    }
    if ((j & 1) == 1) {
        return (seq_t)0; /* odd j -> 0 */
    }

    /* i odd, j even: ((i+1)/2)^(j/2) */
    seq_t base = (seq_t)((i + 1) >> 1); /* (i+1)/2 */
    idx_t exp  = j >> 1;                 /* j/2  */

    seq_t res = (seq_t)1;
    for (idx_t k = 0; k < exp; ++k) {
        res *= base;
    }
    return res;
}
