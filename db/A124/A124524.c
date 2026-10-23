/*
 * A124524  Table (read by antidiagonals) where t(1,1) = 1, t(m,n) = number of
 *          terms above and to the left of t(m,n) (i.e., number of t(k,j)'s,
 *          where 1 <= k <= m, 1 <= j <= n, excluding the t(m,n) case itself)
 *          which both divide m and are coprime to n.
 * https://oeis.org/A124524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table A124524:
   a(n) is the value of t(m,n) in the infinite table,
   where t(1,1)=1, and for m>1 or n>1
   t(m,n) = #{ (k,j) | 1<=k<=m, 1<=j<=n, (k,j)!=(m,n),
                  t(k,j) divides m, gcd(t(k,j),n)=1 }.
   a(n) is obtained by reading the table along antidiagonals
   (sum m+n constant) in descending‑m order.
   The routine builds the table up to the diagonal required for the
   requested index, stores the values, and returns the one that
   corresponds to n.                                 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Symbolic constants for the core types are provided by the build. */
seq_t a(idx_t n)
{
    /* Static cache state */
    static idx_t cache_dim = 0;          /* current maximum coordinate */
    static seq_t *cache_tbl = NULL;      /* table[ (m)*(dim+1)+n ] , 1‑based indices */

    /* ------------------------------------------------------------------ */
    /* Find the diagonal sum S such that S(S-1)/2 >= n                       */
    /* ------------------------------------------------------------------ */
    idx_t S = 2;
    while (S * (S - 1) / 2 < n)
        ++S;
    idx_t dim = S - 1;                    /* maximum coordinate needed */

    /* ------------------------------------------------------------------ */
    /* Rebuild the table if we need a larger dimension.                    */
    /* ------------------------------------------------------------------ */
    if (dim > cache_dim) {
        /* Free old memory if it exists */
        if (cache_tbl) free(cache_tbl);

        /* Allocate new table */
        size_t size = (size_t)(dim + 1) * (dim + 1);   /* indices 0..dim inclusive */
        cache_tbl = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache_tbl) { /* abort on allocation failure */
            return (seq_t)0;
        }

        /* Initialise all entries to zero */
        for (size_t i = 0; i < size; ++i) cache_tbl[i] = 0;

        /* ------------------------------------------------------------------ */
        /* Fill the table in increasing sum order (m+n).  For each cell (m,n)   */
        /* we count earlier cells that divide m and are coprime to n.          */
        /* ------------------------------------------------------------------ */
        for (idx_t sum = 2; sum <= S; ++sum) {          /* sum = m + n, 2 <= sum <= S */
            for (idx_t m = 1; m < sum; ++m) {           /* m ranges from 1 to sum-1 */
                idx_t j = sum - m;                      /* n = sum - m */
                if (m == 1 && j == 1) {
                    cache_tbl[1 * (dim + 1) + 1] = 1;    /* t(1,1) = 1 */
                    continue;
                }
                seq_t count = 0;
                /* iterate over the rectangular prefix */
                for (idx_t k = 1; k <= m; ++k) {
                    size_t k_off = (size_t)k * (dim + 1);
                    for (idx_t jj = 1; jj <= j; ++jj) {
                        if (k == m && jj == j) continue;    /* skip itself */
                        seq_t val = cache_tbl[k_off + jj];
                        /* check divisibility of m by val                    */
                        if (m % (idx_t)val == 0) {
                            /* check coprime of val and n                    */
                            if (gcd(val, (seq_t)j) == 1U) {
                                ++count;
                            }
                        }
                    }
                }
                cache_tbl[m * (dim + 1) + j] = count;
            }
        }
        cache_dim = dim;
    }

    /* ------------------------------------------------------------------ */
    /* Map linear index n to coordinates (m,n) on the antidiagonals.      */
    /* ------------------------------------------------------------------ */
    idx_t prev = (S - 1) * (S - 2) / 2;     /* number of entries before sum S */
    idx_t pos  = n - prev;                  /* 1‑based position in this diagonal */
    idx_t m    = S - pos;                   /* descending m order */
    idx_t j    = pos;

    return cache_tbl[m * (cache_dim + 1) + j];
}
