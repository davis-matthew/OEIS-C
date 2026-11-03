/*
 * A160760  Triangle read by rows, binomial transform of an infinite lower
 *          triangular Toeplitz matrix with A078008 in every column.
 * https://oeis.org/A160760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   mem, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  Triangle T is read by rows.
 *   T(0,0) = 1.
 *   For r ≥ 1:  T(r,0)   = 3^(r-1)            // left border
 *              T(r,r)   = 1                  // right border
 *              T(r,c)   = T(r-1,c) + T(r-1,c-1)  (1 ≤ c < r)
 *   The sequence a(n) is the row‑major traversal of T:
 *   a(n) = T(r,c)  where r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *   c = n - r(r+1)/2.  The sequence begins: 1; 1,1; 3,2,1; 9,5,3,1; …
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *tri = NULL;   /* linear array of triangle values */
    static size_t cap = 0;      /* allocated capacity of tri in seq_t elements */
    static idx_t rows = 0;      /* number of rows already constructed (next row to build) */

    /* Build rows until we have stored a(n) */
    while (1) {
        idx_t r = rows;
        /* starting index of row r: r*(r+1)/2, in idx_t (going through seq_t
           made g++ 15.2 -O2 miscompile the big build: its jump threading
           took the r == 0 branch below for every r) */
        idx_t start = r * (r + 1) / 2;
        /* starting index of previous row (needed for interior entries) */
        idx_t prev_start = (r == 0) ? 0 : (r - 1) * r / 2;

        /* ensure enough storage for the new row (indices up to start+r inclusive) */
        size_t needed = (size_t)(start + r + 1);
        if (cap < needed) {
            size_t new_cap = cap ? cap << 1 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *p = mem_resize(seq_t, tri, new_cap);
            if (!p) abort();
            tri = p;
            cap = new_cap;
        }

        /* leftmost element of the new row */
        seq_t left;
        if (r == 0) {
            left = (seq_t)1;
        } else {
            left = pow3(r - 1);       /* pow3 is a utility that returns 3^(exp) in seq_t type */
        }
        tri[start] = left;

        /* interior elements: sum of two elements above */
        for (idx_t c = 1; c < r; ++c) {
            tri[start + c] = tri[prev_start + c] + tri[prev_start + c - 1];
        }

        /* rightmost element */
        tri[start + r] = (seq_t)1;

        /* row r has just been constructed */
        ++rows;

        /* if the desired index lies within this row, return it */
        if (n <= start + r) {
            return tri[n];
        }
        /* otherwise continue building the next row */
    }
}
