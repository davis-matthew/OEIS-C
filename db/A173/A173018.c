/*
 * A173018  Euler's triangle: triangle of Eulerian numbers T(n,k) (n>=0, 0 <=
 *          k <= n) read by rows.
 * https://oeis.org/A173018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the Eulerian triangle T(n,k) read by rows (T(0,0)=1,
 *            T(n,0)=T(n-1,0), T(n,n)=0, T(n,k)=
 *            (k+1)*T(n-1,k)+(n-k)*T(n-1,k-1) for 1≤k<n)
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 109, u64 n <= 260,
 *            u128 n <= 643, big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Build may compile as C or C++. In the C++ case the result
 * of realloc() is a void*; it must be cast to seq_t*.
 */

#include <stdlib.h>
#include "../utils/offset_row.h"

static seq_t *seq = NULL;          /* linear array holding all computed terms */
static idx_t  max_row = (idx_t)-1; /* highest row that has been fully generated */

/* Ensure that all rows 0..r are stored in seq. */
static void ensure_row(idx_t r)
{
    if (max_row != (idx_t)-1 && r <= max_row)
        return;

    /* new length needed: offset of row r plus (r+1) elements in it */
    size_t new_len = (size_t)offset_row(r) + (size_t)(r + 1);
    seq = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
    if (seq == NULL)   /* allocation failure – unlikely in tests */
        return;

    /* Initialise the very first row if this is the first allocation */
    if (max_row == (idx_t)-1)
        seq[0] = (seq_t)1;   /* T(0,0)=1 */

    /* Begin building from the next uncomputed row */
    idx_t start = (max_row == (idx_t)-1) ? 1 : (max_row + 1);

    for (idx_t i = start; i <= r; ++i) {
        seq_t *prev = &seq[offset_row(i - 1)];
        seq_t *curr = &seq[offset_row(i)];

        /* First entry of the row – stays the same as previous row */
        curr[0] = prev[0];

        /* Interior entries via the recurrence */
        for (idx_t k = 1; k < i; ++k) {
            curr[k] = (seq_t)(k + 1) * prev[k] + (seq_t)(i - k) * prev[k - 1];
        }

        /* Last entry of the row is zero for n>0 */
        curr[i] = (seq_t)0;
    }

    max_row = r;
}

/* Return the n-th term of A173018.  n is 0‑based. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n:
       offset_row(r) <= n < offset_row(r+1) */
    idx_t r = 0;
    while (offset_row(r + 1) <= n) {
        ++r;
    }

    /* Compute rows up to r if necessary */
    ensure_row(r);

    /* Position within the row */
    idx_t k = n - offset_row(r);
    return seq[offset_row(r) + k];
}
