/*
 * A071946  Triangle T(n,k) read by rows giving number of underdiagonal
 *          lattice paths from (0,0) to (n,k) using only steps R = (1,0), V =
 *          (0,1) and D = (3,1).
 * https://oeis.org/A071946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 73, u32 n <= 205, u64 n <= 627,
 *            u128 n <= 2206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A071946
 *  Triangle T(n,k) read by rows gives the number of underdiagonal lattice paths
 *  from (0,0) to (n,k) with steps R=(1,0), V=(0,1) and D=(3,1).
 *
 *  T(0,0) = 1
 *  T(n,k) = T(n,k-1) + T(n-1,k) + T(n-3,k-1)   for n>0, k≥0
 *  T(n,k) = 0                                  if k<0  or k>n
 *
 *  a(n) is the triangle read in row‑major order:
 *  a(n) = T(r,c) where r and c satisfy
 *       r(r+1)/2 ≤ n < (r+1)(r+2)/2   (row number)
 *       c = n – r(r+1)/2              (column within the row)
 *
 *  The function below computes a(n) for any non‑negative n and memoises
 *  the triangle rows so that repeated calls are fast (O(r) time,
 *  with r≈√(2n)).  All arithmetic is done in seq_t, and indices use idx_t.
 */

#include <stdlib.h>

/* Forward declaration of the required types – the build system
   supplies seq_t and idx_t.  Do not define them here. */

seq_t a(idx_t n)
{
    /* Find the row r: the largest r such that r*(r+1)/2 ≤ n. */
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 <= n)
        ++r;
    r--;  /* r is now the correct row number */

    /* Column within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Memoised storage of the triangle rows */
    static seq_t **rows = NULL;       /* array of pointers to rows */
    static idx_t used = 0;            /* number of rows generated so far */
    static idx_t cap  = 0;            /* capacity of the rows array */

    /* Grow the rows array if necessary (doubling strategy) */
    if (used >= cap) {
        size_t new_cap = cap == 0 ? 8 : 2 * cap;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows = tmp;
        cap  = (idx_t)new_cap;
    }

    /* Generate missing rows up to r */
    while (used <= r) {
        /* Allocate the new row (length used+1) */
        seq_t *row = (seq_t *)malloc((used + 1) * sizeof(seq_t));
        if (used == 0) {                  /* base case: T(0,0)=1 */
            row[0] = 1;
        } else {
            for (idx_t j = 0; j <= used; ++j) {
                seq_t val = 0;
                /* T(n,k-1) term */
                if (j > 0)
                    val += row[j - 1];
                /* T(n-1,k) term */
                if (used > 0 && j <= used - 1)
                    val += rows[used - 1][j];
                /* T(n-3,k-1) term */
                if (used >= 3 && j > 0 && j - 1 <= used - 3)
                    val += rows[used - 3][j - 1];
                row[j] = val;
            }
        }
        /* Store the new row in the memoised table */
        if (used >= cap) {          /* extend capacity again if needed */
            size_t new_cap = cap == 0 ? 8 : 2 * cap;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            rows = tmp;
            cap  = (idx_t)new_cap;
        }
        rows[used] = row;
        ++used;
    }

    /* Return the required entry */
    return rows[r][k];
}
