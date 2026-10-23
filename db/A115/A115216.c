/*
 * A115216  "Correlation triangle" for 2^n.
 * https://oeis.org/A115216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = sum_{t=0..m} 2^(r-2*t), where * r = row such that n is in
 *            row r, * k = position in row r (0‑based), * m = min(k, r-k)
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t r = 0;
    idx_t cum = 1;               /* number of elements up to row r */
    while (cum <= n) {
        r++;
        cum += r + 1;             /* append the next row of length r+1 */
    }

    /* Compute the starting index of the current row */
    idx_t row_start = cum - (r + 1);
    idx_t k = n - row_start;     /* position within the row, 0‑based */

    /* m = min(k, r-k) */
    idx_t m = k;
    if (r - k < m) m = r - k;

    /* Sum the powers of two */
    seq_t sum = 0;
    for (idx_t t = 0; t <= m; t++) {
        sum += pow2(r - 2 * t);
    }
    return sum;
}
