/*
 * A076302  Triangle T(n,k) = number of k-smooth divisors of n, read by rows.
 * https://oeis.org/A076302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   row_col, smooth_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the entry of the triangle T(r,k) where
 *   r = smallest integer with r(r+1)/2 ≥ n         (row index)
 *   k = n - (r-1)r/2                               (column index)
 * T(r,k) = number of k‑smooth divisors of r
 *         = ∏_{p^e‖r, p≤k} (e+1)
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_col.h"
#include "../utils/smooth_divisor_count.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col(n, &r, &k);
    return smooth_divisor_count(r, k);
}
