/*
 * A127094  Triangle, reversal of A127093.
 * https://oeis.org/A127094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, K) = mod(n, k-n-1) - mod(n+1, k-n-1) + 1 (reversed rows)
 * Range:     u8 n <= 465, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1, offset 1; return 0 for the unused n == 0 case. */
    if (n == 0)
        return 0;

    /* Find the 1‑based row that contains the nth element. */
    idx_t ri = 0;      /* current row number (1‑based) */
    idx_t cum = 0;     /* total elements up to current row */
    while (cum < n) {
        ++ri;
        cum += ri;
    }

    /* Zero‑based row index and the cumulative count before this row. */
    idx_t row = ri - 1;
    idx_t prev = cum - ri;

    /* Column index within the row (0‑based). */
    idx_t k = n - prev - 1;

    /* Compute T(row, k) using the OEIS formula.
       The divisor is negative; the C remainder operator with a
       negative divisor behaves exactly as defined for this sequence. */
    int64_t denom = (int64_t)k - (int64_t)row - 1;   /* ≤ -1 */
    int64_t rval  = (int64_t)row;
    int64_t a_val = rval % denom;
    int64_t b_val = (rval + 1) % denom;
    int64_t result = a_val - b_val + 1;

    return (seq_t)result;
}
