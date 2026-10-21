/*
 * A224791  Triangle T(n,k) read by rows: left edge is 0, 1, 2, ... (cf.
 *          A001477); otherwise each entry is sum of entry to left and entries
 *          immediately above it to left and right, with 1 for the missing
 *          right term at right edge.
 * https://oeis.org/A224791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 133, u64 n <= 432,
 *            u128 n <= 1536, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle T(n,k) is defined by
 *
 *   T(n,0)   = n
 *   T(n+1,k) = T(n+1,k-1) + T(n,k-1) + T(n,k)   for 0 < k <= n
 *   T(n+1,n+1) = T(n+1,n) + T(n,n) + 1
 *
 *   a(n) is the sequence of diagonal sums of this triangle, read by rows.
 *   The implementation builds the required row of the triangle only once
 *   for each call and extracts the needed element.  It works for any
 *   integer type seq_t supplied by the build (uint8_t … uint64_t, __int128,
 *   or arbitrary‑precision integer class).
 *
 *   The routine uses the helper functions alloc_seq_array and free_seq_array
 *   for proper construction and destruction of seq_t objects in C++ builds.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* Find the row r that contains the index n.
       The total number of entries up to and including row r is
       (r+1)(r+2)/2.  We increase r until that total exceeds n. */
    idx_t r = 0;
    idx_t cum = 1;                 /* terms up to row r (initially r = 0) */
    while (cum <= n) {
        ++r;
        cum += (r + 1);
    }

    /* Calculate the column index k within row r. */
    idx_t prev_total = cum - (r + 1);      /* terms up to row r‑1 */
    idx_t k = n - prev_total;
    if (k > r)  /* safety check, should never happen */
        return (seq_t)0;

    /* Allocate two arrays to hold consecutive rows of the triangle. */
    size_t len = (size_t)(r + 1);
    seq_t *prev = alloc_seq_array(len);
    seq_t *cur  = alloc_seq_array(len);
    if (!prev || !cur) {
        free_seq_array(prev);
        free_seq_array(cur);
        return (seq_t)0;             /* allocation failure: return 0 */
    }

    /* Build the triangle up to row r. */
    for (idx_t row = 0; row <= r; ++row) {
        if (row == 0) {
            cur[0] = (seq_t)0;          /* T(0,0) */
        } else {
            cur[0] = (seq_t)row;        /* left edge T(row,0) */

            for (idx_t j = 1; j <= row; ++j) {
                if (j == row) {         /* right edge: missing right term = 1 */
                    cur[j] = cur[j-1] + prev[j-1] + (seq_t)1;
                } else {                /* general interior */
                    cur[j] = cur[j-1] + prev[j-1] + prev[j];
                }
            }
        }

        if (row == r) {                 /* we have the needed value */
            seq_t result = cur[k];
            free_seq_array(prev);
            free_seq_array(cur);
            return result;
        }

        /* Prepare for next iteration: swap pointers. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Should never reach here, but keep the compiler happy. */
    free_seq_array(prev);
    free_seq_array(cur);
    return (seq_t)0;
}
