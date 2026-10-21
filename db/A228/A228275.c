/*
 * A228275  A(n,k) = Sum_{i=1..k} n^i; square array A(n,k), n>=0, k>=0, read
 *          by antidiagonals.
 * https://oeis.org/A228275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A(row, col) where A(n,k)=sum_{i=1..k} n^i (row=offset,
 *            col=diag-offset)
 * Range:     u8 n <= 38, u16 n <= 81, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 1186, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that the total number of terms up to
       this diagonal exceeds idx.  The number of terms up to and
       including diagonal s is (s+1)(s+2)/2. */
    idx_t s = 0;
    while ((__int128)(s + 1) * (s + 2) / 2 <= (unsigned __int128)n) {
        ++s;
    }

    /* Number of terms strictly before this diagonal. */
    unsigned __int128 prev_terms = (__int128)s * (s + 1) / 2;
    idx_t offset = n - (idx_t)prev_terms;          /* 0 ≤ offset ≤ s */

    /* Coordinates of the required entry within the array. */
    idx_t row = offset;             /* first coordinate (n) */
    idx_t col = s - offset;         /* second coordinate (k) */

    /* Handle trivial cases. */
    if (row == 0 || col == 0) {
        return (seq_t)0;
    }
    if (row == 1) {
        return (seq_t)col;          /* A(1,k) = k */
    }

    /* Compute A(row, col) = sum_{i=1..col} row^i
       Efficiently: ((row^col - 1)/(row-1)) * row.
       The division is performed before the final multiplication
       to avoid overflow when the intermediate product exceeds
       the width of seq_t. */
    seq_t n_seq = (seq_t)row;
    seq_t k_seq = (seq_t)col;
    seq_t pow_row_k = big_pow(n_seq, k_seq);     /* row^col */
    seq_t val = pow_row_k - (seq_t)1;            /* row^col - 1 */
    val = val / (n_seq - (seq_t)1);              /* divide by (row-1) */
    val = val * n_seq;                           /* multiply by row */
    return val;
}
