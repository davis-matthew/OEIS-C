/*
 * A225233  Triangle read by rows: T(n, k) = (2*n + 2 - k)*k, for 0 <= k <= n.
 * https://oeis.org/A225233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (2*row + 2 - col)*col, where row =
 *            floor((sqrt(1+8*n)-1)/2) and col = n - row*(row+1)/2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A225233 */
#pragma once
#include <stdint.h>

/* External linkage function required by the build */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    uint64_t r = 0;
    while ((r * (r + 1)) / 2 <= n) {
        ++r;
    }
    /* r now satisfies the inequality above, so backtrack one step */
    --r;

    /* Compute column k within that row */
    uint64_t k = n - (r * (r + 1)) / 2;

    /* Compute the triangle entry: T(r, k) = (2*r + 2 - k)*k */
    seq_t first_factor  = (seq_t)(2 * r + 2 - k);
    seq_t second_factor = (seq_t)k;
    seq_t result        = first_factor * second_factor;

    return result;
}
