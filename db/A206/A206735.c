/*
 * A206735  Triangle T(n,k), read by rows, given by (0, 2, -1/2, 1/2, 0, 0, 0,
 *          0, 0, 0, 0, ...) DELTA (1, 0, -1/2, 1/2, 0, 0, 0, 0, 0, 0, 0, ...)
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A206735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) = 0^n for k =
 *            0 (1 if n = 0, else 0) binomial(n,k) for k > 0. The sequence is
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0, ... as the first element of each
 *            row gives 0 for all rows except the first. The mapping from the
 *            flattened index n to a pair (row r, col...
 * Range:     u8 n <= 69, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   _row_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_row_of_idx.h"

/* Helper to compute 128‑bit intermediate values safely. */
static seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (unsigned __int128)(n - k + i) / (unsigned __int128)i;
    }
    return (seq_t)res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = _row_of_idx(n);
    /* previous index of the first element of row r */
    idx_t prev = (idx_t)(((unsigned __int128)r * (r + 1)) >> 1);
    idx_t c = n - prev;            /* column within the row */

    if (c == 0)                    /* first element of the row */
        return (r == 0) ? (seq_t)1 : (seq_t)0;

    return _binomial(r, c);
}
