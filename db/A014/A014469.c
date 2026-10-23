/*
 * A014469  Triangular array formed from odd elements to right of middle of
 *          rows of the triangle of Eulerian numbers (A008292).
 * https://oeis.org/A014469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = odd elements to the right of the middle of row n of the
 *            Eulerian numbers triangle A008292
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the Eulerian triangle row by row and returns the nth odd element found
   to the right of the midpoint of each row, counting rows in the usual 0‑based order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* The sequence is 1‑based; a(0) is unused */

    /* Static state to allow out‑of‑order calls and memoisation */
    static seq_t *prev = NULL;      /* previous row of Eulerian numbers */
    static size_t prev_len = 0;     /* length of prev (prev_len = last_row + 1) */
    static seq_t *cache = NULL;     /* flattened list of odd elements found so far */
    static size_t cache_len = 0;    /* number of cached elements */
    static size_t cache_cap = 0;    /* capacity of cache */
    static idx_t last_row = 0;      /* last row number that has been computed (0‑based) */

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Initialise the first row (row 0 of the Eulerian triangle) if necessary */
    if (!prev) {
        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev[0] = 1;
        last_row = 0;
    }

    /* Generate rows until enough odd elements are collected */
    while (cache_len < (size_t)n) {
        idx_t r = last_row + 1;                /* index of the new row (1‑based) */
        size_t len = (size_t)(r + 1);          /* number of elements in this row */

        /* Allocate space for the new row */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) return 0;                   /* allocation failed – return 0 as a safe value */

        /* Boundary values of each Eulerian row are 1 */
        curr[0] = 1;
        curr[r] = 1;

        /* Compute interior values using the recurrence:
           curr[k] = (k+1)*prev[k] + (r+1-k)*prev[k-1]  for 1 ≤ k < r
           indices for prev are cast to size_t for array access */
        for (idx_t k = 1; k < r; ++k) {
            seq_t term1 = ((seq_t)(k + 1)) * prev[(size_t)k];
            seq_t term2 = ((seq_t)(r + 1 - k)) * prev[(size_t)(k - 1)];
            curr[(size_t)k] = term1 + term2;
        }

        /* Scan the part of the row to the right of its middle, keeping only odd values. */
        idx_t start = (r / 2) + 1;              /* first index strictly to the right of the middle */
        for (idx_t k = start; k <= r; ++k) {
            if ((curr[(size_t)k] & (seq_t)1) != 0) {    /* odd check */
                /* Grow cache if necessary */
                if (cache_len >= cache_cap) {
                    cache_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = curr[(size_t)k];
            }
        }

        /* Prepare for the next iteration: the current row becomes the previous row. */
        free(prev);
        prev = curr;
        prev_len = len;
        last_row = r;
    }

    return cache[n - 1];
}
