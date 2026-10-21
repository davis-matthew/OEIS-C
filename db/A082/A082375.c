/*
 * A082375  Irregular triangle read by rows: row n begins with n and decreases
 *          by 2 until 0 or 1 is reached, for n >= 0.
 * https://oeis.org/A082375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 245, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   C_of_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A082375: flatten irregular triangle T(n,m) = n-2*m for
   0 ≤ m ≤ ⌊n/2⌋.  a(n) returns the n‑th element (0‑indexed) of that
   triangle.  The algorithm finds the row r such that
   C(r‑1) ≤ n < C(r) where C(r) is the total number of elements up to
   row r, then evaluates T(r,n‑C(r‑1)).  C(r) is computed directly by
   a closed form, so the routine runs in O(r) ≈ O(√n) time. */

#include <stdint.h>
#include "../utils/C_of_r.h"

seq_t a(idx_t n)
{
    /* Find the first r such that C_of_r(r) > n. */
    idx_t r = 0U;
    while (1) {
        seq_t c = C_of_r(r);
        if (c > (seq_t)n) break;
        ++r;
    }

    /* Index of first element in row r */
    idx_t start = 0U;
    if (r != 0U)
        start = (idx_t)C_of_r(r - 1);

    /* Position within the row (0‑based) */
    idx_t pos = n - start;

    /* T(r,pos) = r - 2*pos */
    return (seq_t)r - (seq_t)((idx_t)2 * pos);
}
