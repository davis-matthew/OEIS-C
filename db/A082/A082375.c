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
 * Validated: standard, pass, 109 terms through n = 968 (2026-11-01)
 * Helpers:   none
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

static inline seq_t C_of_r(idx_t r)
{
    /* Total elements in rows 0..r inclusive.
       Formula depends on parity of r. */
    if (r & 1U) {                        /* r = 2*m+1 (odd) */
        idx_t m = (r - 1U) >> 1;
        seq_t mm = (seq_t)m;
        return mm * mm + (seq_t)3 * mm + (seq_t)2;
    } else {                              /* r = 2*m (even) */
        idx_t m = r >> 1;
        seq_t mm = (seq_t)m;
        return mm * mm + (seq_t)2 * mm + (seq_t)1;
    }
}

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
