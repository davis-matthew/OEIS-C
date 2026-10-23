/*
 * A156919  Table of coefficients of polynomials related to the Dirichlet eta
 *          function.
 * https://oeis.org/A156919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A156919
   Triangle recurrence:
     T(0,0) = 1
     T(n,0) = 2^n
     T(n,n) = 1
     T(n,m) = (2*m+2)*T(n-1,m) + (2*n-2*m+1)*T(n-1,m-1)   for 1 ≤ m ≤ n-1
   The sequence a(n) is obtained by listing the triangle row‑by‑row
   (row 0 first, then row 1 left to right, etc.).
*/

#include <stdlib.h>

/* static cache of rows of the triangle */
static seq_t **row_cache   = NULL; /* array of pointers to rows */
static size_t  cache_cap   = 0;    /* allocated capacity for row_cache */
static size_t  cache_size  = 0;    /* number of rows currently stored */

/* enlarge row_cache to hold at least 'needed' rows */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= needed)
        new_cap <<= 1; /* double until large enough */
    seq_t **new_cache = (seq_t **)realloc(row_cache,
                                          new_cap * sizeof(seq_t *));
    if (!new_cache)
        exit(EXIT_FAILURE); /* out of memory */
    row_cache = new_cache;
    cache_cap = new_cap;
}

/* external function that returns the n-th term of A156919 */
seq_t a(idx_t n)
{
    /* determine the row r and column m that correspond to index n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r; /* now r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t offset = (r * (r + 1)) / 2;
    idx_t m = n - offset; /* column within the row */

    /* Ensure rows up to r are computed and cached */
    if (cache_size <= (size_t)r)
    {
        grow_cache(r + 1); /* allocate space for all needed rows */

        /* compute rows from cache_size up to r */
        for (size_t i = cache_size; i <= (size_t)r; ++i)
        {
            size_t len = i + 1;            /* length of row i */
            seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
            if (!row)
                exit(EXIT_FAILURE); /* out of memory */

            if (i == 0)
            {
                /* base row T(0,0) = 1 */
                row[0] = 1;
            }
            else
            {
                seq_t *prev = row_cache[i - 1];
                /* first element T(i,0) = 2*T(i-1,0) = 2^i */
                row[0] = prev[0] * 2;
                /* last element T(i,i) = 1 */
                row[i] = 1;
                /* inner elements */
                for (idx_t j = 1; j < i; ++j)
                {
                    seq_t term1 = ((seq_t)(2 * j + 2)) * prev[j];
                    seq_t term2 = ((seq_t)(2 * i - 2 * j + 1)) * prev[j - 1];
                    row[j] = term1 + term2;
                }
            }
            row_cache[i] = row;
        }
        cache_size = r + 1;
    }

    return row_cache[(size_t)r][(size_t)m];
}
