/*
 * A173333  Triangle read by rows: T(n, k) = n! / k!, 1 <= k <= n.
 * https://oeis.org/A173333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = r! / k! * where r is the unique integer such that *
 *            r(r-1)/2 < n ≤ r(r+1)/2 * and k = n - r(r-1)/2 (the column index
 *            in row r). * This corresponds to the triangle read by rows: *
 *            T(n,k) = n! / k!, 1 ≤ k ≤ n
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th term. */
    idx_t r = 1;
    idx_t cum = 1;                    /* cumulative count up to row r */
    while (cum < n) {
        ++r;
        cum = r * (r + 1) / 2;
    }

    /* Determine the column index k within row r. */
    idx_t prev_cum = (r - 1) * r / 2;
    idx_t k = n - prev_cum;           /* 1‑based column in the row */

    /* Compute T(r,k) = r! / k! by multiplying the integers k+1 … r. */
    seq_t result = 1;
    for (idx_t i = k + 1; i <= r; ++i) {
        result = seq_mul(result, (seq_t)i);
    }
    return result;
}
