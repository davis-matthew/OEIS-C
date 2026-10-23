/*
 * A152667  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k runs of even entries (n >= 2, 1 <= k <=
 *          floor(n/2)). For example, the permutation 321756498 has 3 runs of
 *          even entries: 2, 64 and 8.
 * https://oeis.org/A152667
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   S_of_row, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A152667 – Flattened triangle T(n,k)
    T(2m,k)   = (m!)² * C(m+1,k) * C(m-1,k-1)
    T(2m+1,k) = m! * (m+1)! * C(m-1,k-1) * C(m+2,k)
    a(n) is the n‑th entry of the triangle read row‑by‑row
    (offset 2, so a(2) = first entry).                                       */

#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include <stddef.h>
#include <stdint.h>
#include "../utils/S_of_row.h"

/* Cumulative count of triangle entries up to row r (inclusive).          */

/* Convert global index n (offset 2) to the row r and column k of the triangle */
static void row_col_from_index(idx_t pos, idx_t *row, idx_t *col)
{
    /* Binary search for smallest r with S(r) ≥ pos (pos is 1‑based) */
    idx_t low = 2, high = 2;
    while (S_of_row(high) < pos)
        high <<= 1;                      /* expand until high covers pos */

    while (high - low > 1)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (S_of_row(mid) >= pos)
            high = mid;
        else
            low = mid;
    }
    *row = high;
    *col = pos - S_of_row(high - 1);      /* 1‑based column within the row */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)          /* undefined for indices below the offset */
        return 0;

    /* Position of the required entry in the flattened list (1‑based).    */
    idx_t pos = n - 1;  /* because a(2) is the first entry */

    idx_t r, k;
    row_col_from_index(pos, &r, &k);

    /* Compute T(r,k) according to the parity of r */
    if ((r & 1) == 0) /* row r = 2m */
    {
        idx_t m = r >> 1;
        seq_t fac_m = factorial(m);
        seq_t term   = fac_m * fac_m;           /* (m!)² */
        term = term * binomial(m + 1, k);        /* * C(m+1,k) */
        term = term * binomial(m - 1, k - 1);    /* * C(m-1,k-1) */
        return term;
    }
    else /* row r = 2m+1 */
    {
        idx_t m = (r - 1) >> 1;                 /* m = (r-1)/2 */
        seq_t fac_m  = factorial(m);
        seq_t fac_m1 = factorial(m + 1);
        seq_t term   = fac_m * fac_m1;           /* m! * (m+1)! */
        term = term * binomial(m - 1, k - 1);    /* * C(m-1,k-1) */
        term = term * binomial(m + 2, k);        /* * C(m+2,k) */
        return term;
    }
}
