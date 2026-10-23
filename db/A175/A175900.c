/*
 * A175900  Number of lattice paths from (0,0) to (n,n) using steps
 *          S={(k,0),(0,k),(1,1)|0<k<=4} which never go above the line y=x.
 * https://oeis.org/A175900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lattice paths from (0,0) to (n,n) with steps
 *            (k,0),(0,k),(1,1) (1≤k≤4) that never go above the diagonal y=x.
 *            Computation is performed by dynamic programming over the
 *            difference d = x−y, keeping the last four rows of the table. The
 *            algorithm is O(n²) in time and O(n) in memory
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Static data for memoization and incremental computation */
    static idx_t  max_n      = 0;          /* largest n processed so far    */
    static seq_t *rows[5]   = {NULL};      /* circular buffer of last 5 rows*/
    static size_t rows_len  = 0;           /* length allocated for each row */
    static seq_t *a_vals    = NULL;        /* array of already computed a(n) values */
    static size_t a_len     = 0;           /* number of a_vals entries stored */

    /* Ensure a_vals large enough and base case a(0)=1 */
    if (a_len == 0) {
        a_len = 1;
        a_vals = (seq_t *)malloc(a_len * sizeof(seq_t));
        a_vals[0] = (seq_t)1;
    }
    if (n >= a_len) {
        a_vals = (seq_t *)realloc(a_vals, (n + 1) * sizeof(seq_t));
        for (idx_t i = a_len; i <= n; ++i)
            a_vals[i] = (seq_t)0;
        a_len = n + 1;
    }
    if (n == 0)
        return a_vals[0];

    /* Allocate or reallocate rows arrays */
    if (rows_len < (size_t)(n + 1)) {
        rows_len = (size_t)(n + 1);
        for (int i = 0; i < 5; ++i) {
            if (rows[i] == NULL)
                rows[i] = (seq_t *)malloc(rows_len * sizeof(seq_t));
            else
                rows[i] = (seq_t *)realloc(rows[i], rows_len * sizeof(seq_t));
        }
    }

    /* Initialise base row (x=0) if it hasn't been done yet */
    if (max_n == 0) {
        for (int i = 0; i < 5; ++i)
            memset(rows[i], 0, rows_len * sizeof(seq_t));
        rows[0][0] = (seq_t)1;
    }

    /* Compute rows incrementally from max_n+1 to n */
    for (idx_t x = max_n + 1; x <= n; ++x) {
        seq_t *cur = rows[x % 5];
        /* No need to zero elements beyond current x; they will be
           overwritten as we compute. */
        for (idx_t y = 0; y <= x; ++y) {
            seq_t sum = (seq_t)0;

            /* Diagonal step from (x-1,y-1) */
            if (y > 0)
                sum += rows[(x - 1) % 5][y - 1];

            /* Horizontal steps of length k=1..4, possible only if y <= x-k */
            for (idx_t k = 1; k <= 4 && k <= x; ++k) {
                if (y <= x - k)
                    sum += rows[(x - k) % 5][y];
            }

            /* Vertical steps of length k=1..4 */
            for (idx_t k = 1; k <= 4 && k <= y; ++k) {
                sum += cur[y - k];
            }

            cur[y] = sum;
        }
        a_vals[x] = cur[x];  /* f(x,x) is the desired a(x) */
    }
    max_n = n;
    return a_vals[n];
}
