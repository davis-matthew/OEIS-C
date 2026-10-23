/*
 * A130124  Triangle defined by A130123 * A002260, read by rows.
 * https://oeis.org/A130124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r-1) * k, where * r = minimal integer with r*(r+1)/2 ≥
 *            n (row number), * k = n - (r-1)*r/2 (column number)
 * Range:     u8 n <= 24, u16 n <= 98, u32 n <= 421, u64 n <= 1801,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((seq_t)r * ((seq_t)r + 1) / 2 < n)
        r++;

    /* Column index k in this row */
    idx_t prev_tri = (idx_t)((seq_t)(r - 1) * (seq_t)r / 2);
    idx_t k = n - prev_tri;

    /* Compute 2^(r-1) and multiply by k */
    seq_t val = pow2(r - 1);
    val = val * (seq_t)k;
    return val;
}
