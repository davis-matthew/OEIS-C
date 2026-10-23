/*
 * A176227  A symmetrical triangle sequence: T(n, k) = q^k + q^(n-k) - q^n,
 *          with q=4.
 * https://oeis.org/A176227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = 4^k + 4^(r‑k) – 4^r, where n = r*(r+1)/2 + k (0 <= k <=
 *            r)
 * Range:     i8 n <= 10, i16 n <= 36, i32 n <= 136, i64 n <= 528,
 *            i128 n <= 2080, big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        r++;

    /* Column k is the offset within the row */
    idx_t offset = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - offset;

    /* Compute the triangle entry */
    seq_t term = pow4(k) + pow4(r - k) - pow4(r);
    return term;
}
