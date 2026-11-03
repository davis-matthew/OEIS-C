/*
 * A158117  Triangle T(n, k) = 10^(k*(n-k)), read by rows.
 * https://oeis.org/A158117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 1284 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158117: a(n) = 10^(k * (r - k)) where
   r is the row index satisfying r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
   and k = n - r*(r+1)/2 is the column index within that row. */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row index r such that T(r) = r*(r+1)/2 ≤ n < T(r+1). */
    idx_t r = 0;
    for (;; ++r) {
        idx_t t = r * (r + 1) / 2;
        if (t > n) {
            r--;          /* r is now the largest satisfying r*(r+1)/2 ≤ n */
            break;
        }
    }

    /* Cumulative count before this row. */
    idx_t prev = r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t k = n - prev;

    /* Exponent for the current entry. */
    idx_t e = k * (r - k);

    return pow10(e);   /* pow10 defined in ../utils/pow10.h */
}
