/*
 * A131035  Triangle read by rows: A051340 * A129686 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A131035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 107 terms through n = 1307 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131035: flattened entry of the product of A051340 and A129686 as lower triangular matrices.
   The triangle entry T(i,j) for i ≥ j ≥ 0 is given by
        T(i,j) = (i-j+1)
                + (i>=2 && j<=i-2 ? (i-j-1) : 0)
                - (i>=2 && j>=1   ? (i-j   ) : 0)
                - (i>=3 && j<=i-3 ? (i-j-2) : 0)
   The flattened index is a(n) = T(i,j) where i is the row number and j the column
   obtained from n via the standard mapping n = i(i+1)/2 + j.  */
#include <stdint.h>
#include <stddef.h>

/* Helper: compute i (row) and j (column) from flattened index n */
static void _ij_from_n(idx_t n, idx_t *ip, idx_t *jp)
{
    /* Find i such that i(i+1)/2 ≤ n < (i+1)(i+2)/2 */
    idx_t low = 0, high = 1;
    /* double high until it surpasses n */
    while (( (__int128)high * (high + 1) ) >> 1 <= n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (( (__int128)mid * (mid + 1) ) >> 1 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t i = low;
    idx_t start = (idx_t)(((__int128)i * (i + 1)) >> 1);
    idx_t j = n - start;
    *ip = i;
    *jp = j;
}

/* Compute triangle entry T(i,j) (i≥j) */
static seq_t _T(idx_t i, idx_t j)
{
    seq_t val = (seq_t)(i - j + 1);

    if (i >= 2 && j <= i - 2)
        val += (seq_t)(i - j - 1);

    if (i >= 2 && j >= 1)
        val -= (seq_t)(i - j);

    if (i >= 3 && j <= i - 3)
        val -= (seq_t)(i - j - 2);

    return val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    _ij_from_n(n, &i, &j);
    return _T(i, j);
}
