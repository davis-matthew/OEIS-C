/*
 * A203905  Symmetric matrix based on (1,0,1,0,1,0,1,0,...), by antidiagonals.
 * https://oeis.org/A203905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = M[i][j] where (i,j) is the position of n on the
 *            antidiagonals of M, with M[i][j] = sum_{k=1}^{min(i,j)} s[i-k+1]
 *            * s[j-k+1] and s is the periodic sequence (1,0,1,0...). The
 *            resulting value is 0 if i and j have different parity; otherwise
 *            it is ceil(min(i,j)/2) when i is odd, or min(i,j)/2...
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   _find_diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_find_diagonal.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but guard against invalid input */

    idx_t k = _find_diagonal(n);                     /* diagonal index (i+j) */
    idx_t prev = ((k - 1) * (k - 2)) >> 1;           /* total elements before this diagonal */
    idx_t offset = n - prev - 1;                     /* 0-based offset within diagonal */
    idx_t i = offset + 1;                            /* row index */
    idx_t j = k - i;                                 /* column index */
    idx_t m = (i < j) ? i : j;                      /* min(i,j) */

    if ((i & 1) != (j & 1))
        return 0;                                   /* parity mismatch -> zero */

    if (i & 1) { /* i odd */
        /* ceil(m/2) = (m + 1)/2 */
        return (seq_t)(m + 1) / 2;
    } else { /* i even */
        return (seq_t)(m) / 2;
    }
}
