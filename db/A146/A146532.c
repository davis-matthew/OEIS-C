/*
 * A146532  Triangle read by rows: a(n)=2^Floor[n/2]*Gamma[1 + Floor[n/2]];
 *          t(n,m) = a[n]/(a[n - m]*a[m]).
 * https://oeis.org/A146532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(row, col) where row satisfies row*(row+1)/2 <= n <
 *            (row+1)*(row+2)/2 and col = n - row*(row+1)/2. t(row, col) =
 *            a[row] / (a[row - col] * a[col]), with a[k] = 2^floor(k/2) *
 *            floor(k/2)!
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* find an upper bound for the row */
    while (high * (high + 1) / 2 <= n)
    {
        low = high;
        high = high * 2 + 1;
    }
    /* binary search for the exact row */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        idx_t off = mid * (mid + 1) / 2;
        if (off <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* locate the row and column */
    idx_t row = find_row(n);
    idx_t offset = row * (row + 1) / 2;
    idx_t col = n - offset;

    /* helper values */
    idx_t k_r   = row / 2;
    idx_t k_c   = col / 2;
    idx_t k_rc  = (row - col) / 2;

    /* compute 2-exponent factor */
    idx_t exp2 = k_r - k_c - k_rc;

    /* calculate factorial ratio k_r! / (k_c! * k_rc!) */
    seq_t res = 1;
    for (idx_t i = 1; i <= k_r; ++i) res *= (seq_t)i;
    for (idx_t i = 1; i <= k_c; ++i) res /= (seq_t)i;
    for (idx_t i = 1; i <= k_rc; ++i) res /= (seq_t)i;

    /* multiply by 2^exp2 */
    res *= pow2((idx_t)exp2);
    return res;
}
