/*
 * A082039  Symmetric square array defined by T(n,k) = k^2*n^2 + k*n + 1, read
 *          by antidiagonals.
 * https://oeis.org/A082039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) reads the square array T(n,k)=k^2*n^2 + k*n + 1 * by
 *            antidiagonals. For n>=0, let d be the diagonal index such that *
 *            d*(d+1)/2 <= n < (d+1)*(d+2)/2. * Inside that diagonal the
 *            offset t = n - d*(d+1)/2 gives the row index * i = t and column
 *            index k = d - t. The term is then * a(n) = k^2*i^2...
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   diag_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/diag_from_index.h"

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Find the diagonal and the offset within that diagonal. */
    idx_t d = diag_from_index(n);
    idx_t cum = (d * (d + 1)) / 2;            /* terms before diagonal d */
    idx_t t = n - cum;                         /* offset inside diagonal */
    idx_t i = t;                               /* row index */
    idx_t k = d - t;                           /* column index */

    /* All arithmetic is done in seq_t to preserve exactness. */
    seq_t n_val = (seq_t)i;
    seq_t k_val = (seq_t)k;
    seq_t kn = k_val * n_val;                  /* k*n */
    seq_t val = kn * kn;                       /* (k*n)^2 == k^2*n^2 */
    val += kn;                                 /* + k*n */
    val += 1;                                   /* + 1 */

    return val;
}
