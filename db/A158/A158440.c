/*
 * A158440  Triangle T(n,k) read by rows: row n contains n times n+1 followed
 *          by n 1's.
 * https://oeis.org/A158440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = row_index(n) + 1 if the element lies in the first * half
 *            of its row; otherwise a(n) = 1. * where row_index(n) is the
 *            smallest r such that r(r+1) ≥ n. * Row r contains r copies of
 *            (r+1) followed by r copies of 1
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Determine the row that contains the n-th term (offset 1). */
static inline idx_t row_of(idx_t n)
{
    idx_t r = 1;
    /* r is at most n for 1‑based offset; for small n (≤10000) this loop
       is trivial and avoids any overflow concerns. */
    while (r * (r + 1) < n)
        ++r;
    return r;
}

/* Core computation of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)         /* 0 is outside the defined offset; return 0. */
        return (seq_t)0;

    idx_t r = row_of(n);
    /* Number of elements before this row: (r-1)*r. */
    idx_t prev = (r - 1) * r;
    idx_t pos  = n - prev;               /* 1‑based position inside the row. */
    return (pos <= r) ? ((seq_t)(r + 1)) : (seq_t)1;
}
