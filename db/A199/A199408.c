/*
 * A199408  Triangle T(n,k) = n + k - gcd(n,k) read by rows, 1 <= n, 1 <= k <=
 *          n.
 * https://oeis.org/A199408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8383, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 88 terms through n = 8349 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A199408:  T(n,k) = n + k – gcd(n,k), read row‑by‑row
    (row r contains k = 1 … r).  For a 1‑based linear index m the
    function returns the mth triangle entry.                          */
#include <stdint.h>
#include "../utils/gcd_idx.h"

/* Euclidean algorithm for idx_t (uint64_t).  Not in the utils list */

seq_t a(idx_t n)
{
    /* Find the row r such that the triangular number r(r+1)/2 
       is the first position of the next row.  We grow r until
       the accumulated count reaches or passes n.              */
    idx_t r    = 1;          /* current row number */
    idx_t sum  = 1;          /* triangular number r(r+1)/2 for current r */

    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Starting index (1‑based) of the found row */
    idx_t start = sum - r + 1;

    /* Column index k within the row (1‑based) */
    idx_t k = n - start + 1;

    /* Compute the triangle value */
    idx_t g = gcd_idx(r, k);

    return (seq_t)r + (seq_t)k - (seq_t)g;
}
