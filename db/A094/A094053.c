/*
 * A094053  Triangle read by rows: T(n,k) = k*(n-k), 1 <= k <= n.
 * https://oeis.org/A094053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k*(n-k) where (n,k) are the row and column indices
 *            obtained from the triangular enumeration of the numbers k*(n-k),
 *            1≤k≤n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   _row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_row_col.h"

/* Sequence A094053 */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1 only   */
        return 0;

    idx_t r, k;
    _row_col(n, &r, &k);

    /* compute k*(r-k) in the target integer type */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return sk * (sr - sk);
}
