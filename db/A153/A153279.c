/*
 * A153279  Eigentriangle by rows, T(n,k) = A000079(n-k) * (diagonalized
 *          matrix of (1,1,3,9,27,81,...)).
 * https://oeis.org/A153279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) * where row(n) is the unique integer with *
 *            offset_row(row) <= n < offset_row(row+1), * offset_row(r) =
 *            r*(r+1)/2, * col(n) = n - offset_row(row), * and * T(r,c) =
 *            2^(r-c) * (c==0 ? 1 : 3^(c-1)). * This yields the triangle read
 *            by rows: 1; 2,1; 4,2,3; 8,4,6,9; …
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   pow2, pow3, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/triangular.h"

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until it passes n */
    while (triangular(high) <= n)
        high <<= 1;
    /* Binary search between low (<= n) and high (> n) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;            /* row such that offset_row(row) <= n < offset_row(row+1) */
}

seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t col = n - triangular(row);

    seq_t pow2_val = pow2(row - col);            /* 2^(row-col)    */
    if (col == 0)
        return pow2_val;                         /* multiply by 1   */
    seq_t pow3_val = pow3(col - 1);              /* 3^(col-1)      */
    return pow2_val * pow3_val;                  /* multiplication is exact in seq_t */
}
