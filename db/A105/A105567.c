/*
 * A105567  Moebius transform of multiplication table, read by antidiagonals.
 * https://oeis.org/A105567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = -μ(k*(row-k)) where row = row_of_index(n) is such that
 *            row*(row+1)/2 ≤ n < (row+1)*(row+2)/2 k = n - row*(row+1)/2
 *            (column index within the row). For k* (row-k) = 0 we take μ(0) =
 *            -1 to obtain a value of +1
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   mobius, row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of_index.h"   /* provides idx_t row_of_index(idx_t n);   */
#include "../utils/mobius.h"        /* provides int mobius(idx_t k);          */

seq_t a(idx_t n)
{
    idx_t row = row_of_index(n);
    idx_t col = n - row * (row + 1) / 2;
    idx_t prod = col * (row - col);      /* k*(row-k) */

    if (prod == 0)                       /* μ(0) is treated as -1 */
        return (seq_t)1;
    else
        return -(seq_t)mobius(prod);     /* -μ(prod) */
}
