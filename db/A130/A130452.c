/*
 * A130452  Triangle read by rows: A097806 * A130321 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A130452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(r - k + 1) where r is the row number such that r(r+1)/2
 *            ≥ n, k = n - (r-1)r/2 is the position within the row, and t(1) =
 *            1, t(q) = 3 * 2^(q-2) for q ≥ 2
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Find the row r that contains index n.  Sum = r(r+1)/2. */
    idx_t r   = 1;
    idx_t sum = 1;                  /* sum of the first r rows for r=1 */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* sum now equals r(r+1)/2.  sum_prev is sum of first (r-1) rows. */
    idx_t sum_prev = sum - r;       /* (r-1)r/2 */
    idx_t k        = n - sum_prev;  /* 1‑based position in row r */
    idx_t q        = r - k + 1;      /* index of the base term t(q) */

    if (q == 1) return (seq_t)1;

    seq_t result = (seq_t)3;
    /* Compute 3 * 2^(q-2) by repeated multiplication.
       Using multiplication avoids undefined shift counts for large q. */
    for (idx_t i = 0; i < q - 2; ++i)
        result *= (seq_t)2;

    return result;
}
