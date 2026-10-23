/*
 * A177823  Triangle of Eulerian numbers squared: A008292(n,m)^2 read by rows.
 * https://oeis.org/A177823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A008292(r, j-1)^2 where r is the unique integer such that
 *            r*(r+1)/2 ≥ n, t = (r-1)*r/2, j = n - t, 1 ≤ j ≤ r, and
 *            A008292(r, k) is the Eulerian number at row r, column k
 *            (0‑based)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 39, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Eulerian numbers are computed using the recurrence
   A(n,k) = (k+1)·A(n-1,k) + (n-k)·A(n-1,k-1) with A(1,0)=1. */
seq_t a(idx_t n)
{
    /* find the row r containing the n‑th term */
    idx_t r = 0;
    while (((seq_t)r * ((seq_t)r + 1)) / 2 < n)
        ++r;

    /* position of n within row r (1‑based) */
    seq_t t_prev = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t j      = n - (idx_t)t_prev;          /* 1 ≤ j ≤ r */
    idx_t idx    = j - 1;                       /* 0‑based index for Eulerian number */

    /* special case: first row */
    if (r == 1)
        return (seq_t)1;

    /* allocate array for previous row (size 1) */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    prev[0] = (seq_t)1;                     /* A(1,0) = 1 */

    /* iterate rows 2 … r */
    for (idx_t cur = 2; cur <= r; ++cur)
    {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * cur);

        for (idx_t m = 0; m < cur; ++m)
        {
            seq_t part1 = 0;
            seq_t part2 = 0;

            if (m < cur - 1)                 /* A(n-1,k) exists */
                part1 = ((seq_t)(m + 1)) * prev[m];

            if (m > 0)                       /* A(n-1,k-1) exists */
                part2 = ((seq_t)(cur - m)) * prev[m - 1];

            curr[m] = part1 + part2;
        }

        free(prev);
        prev = curr;
    }

    /* result is the square of the desired Eulerian number */
    seq_t val      = prev[idx];
    seq_t result   = val * val;

    free(prev);
    return result;
}
