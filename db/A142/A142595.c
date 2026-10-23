/*
 * A142595  Triangle T(n,k) = 2*T(n-1, k-1) + 2*T(n-1, k), read by rows.
 * https://oeis.org/A142595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 180, u64 n <= 644,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  Triangle T(n,k) defined by
   T(1,0) = 1,
   T(n,0)=T(n,n-1)=1  for n>1,
   T(n,k) = 2*T(n-1,k-1) + 2*T(n-1,k)   for 1 ≤ k ≤ n-2.
   The sequence a(n) is the n‑th element when the rows of T are read
   consecutively, starting with a(1)=1. */

#include <stddef.h>

static seq_t prev[2000];
static seq_t curr[2000];

seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == 1)
        return 1;

    /* Find the row r that contains the n-th term. */
    idx_t r   = 1;
    idx_t cum = 1;               /* cumulative number of terms up to row r */
    while (cum < n) {
        r++;
        cum += r;
    }

    /* Position within the found row.  Positions are 1-based. */
    idx_t pos = n - (cum - r);   /* 1 ≤ pos ≤ r */

    /* Build the triangle row by row up to row r. */
    prev[0] = 1;                  /* row 1 */
    for (idx_t i = 2; i <= r; ++i) {
        curr[0] = 1;              /* left edge */
        curr[i-1] = 1;            /* right edge */
        for (idx_t j = 1; j < i-1; ++j) {
            /* curr[j] = 2*prev[j-1] + 2*prev[j]; */
            curr[j] = prev[j-1] + prev[j-1];   /* 2*prev[j-1] */
            curr[j] += prev[j] + prev[j];     /* +2*prev[j] */
        }
        /* Copy curr to prev for the next iteration. */
        for (idx_t j = 0; j < i; ++j)
            prev[j] = curr[j];
    }

    /* Return the desired element in the final row. */
    return prev[pos-1];
}
