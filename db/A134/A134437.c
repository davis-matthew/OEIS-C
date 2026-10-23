/*
 * A134437  Number of cells in the 2nd rows of all deco polyominoes of height
 *          n. A deco polyomino is a directed column-convex polyomino in which
 *          the height, measured along the diagonal, is attained only in the
 *          last column.
 * https://oeis.org/A134437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(3*n-2)*(n-1)*(n-1)! (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) /* undefined below the offset; return 0 for safety */
        return (seq_t)0;

    /* Compute (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i) {
        fact *= (seq_t)i;        /* fact *= i;  i is an idx_t cast to seq_t */
    }

    /* Compute (1/4)*(3*n-2)*(n-1)*fact */
    seq_t tmp = (seq_t)(3 * n - 2);   /* 3*n-2 as seq_t */
    tmp *= (n - 1);                  /* multiply by (n-1) */
    tmp *= fact;                     /* multiply by (n-1)! */
    tmp /= (seq_t)4;                 /* exact division by 4 */

    return tmp;
}
