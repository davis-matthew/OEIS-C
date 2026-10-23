/*
 * A130831  Irregular triangle read by rows: row(n) contains the first
 *          A001146(n) terms of A001316.
 * https://oeis.org/A130831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS sequence A130831.
   The triangle rows are defined as:
   row(n) consists of the first A001146(n) terms of A001316.
   A001316 is given by
        a(m) = 2^popcount(m-1)   (m ≥ 1).
   A001146 satisfies
        A001146(1) = 2,
        A001146(n) = A001146(n-1)^2   (n ≥ 2)   (i.e. 2^(2^(n-1))).
   Thus the whole sequence a(n) is obtained by reading the rows
   row by row.  For a requested index n we locate the row r such
   that the cumulative sum of lengths up to r-1 is < n ≤ cumulative length
   up to r, compute the offset within the row, and return the
   corresponding A001316 term.
   The formula used for the output value is
        a(n) = 2^popcount(k-1)   where k = offset+1.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the row r containing the n‑th term. */
    idx_t len = 2;          /* A001146(1) */
    idx_t cum = 0;          /* cumulative length up to previous row */
    while (true) {
        cum += len;
        if (n <= cum) break;             /* row found */
        /* compute next row length, stop if it would exceed 64‑bit range
           but this is safe for n up to at least 10000 */
        len = len * len;
    }
    idx_t prev_cum = cum - len;           /* cumulative length up to row r–1 */
    idx_t offset = n - prev_cum - 1;      /* zero‑based index within the row */

    /* A001316 term: 2^popcount(offset). */
    /* popcount on the 64‑bit representation of offset */
    uint64_t val64 = (uint64_t)offset;
    unsigned pc = __builtin_popcountll(val64);

    /* Compute 2^pc in seq_t.  All seq_t types support shift. */
    seq_t result = 1;
    result <<= pc;
    return result;
}
