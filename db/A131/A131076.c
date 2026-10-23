/*
 * A131076  Row sums of triangular array T: T(j,1) = 1 for ((j-1) mod 8) < 4,
 *          else 0; T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 <= k <= j.
 * https://oeis.org/A131076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} T(n,k) where T(j,1) = 1 if ((j-1) mod 8) <
 *            4, else 0; T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 <= k <= j
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The following static data allow memoisation between calls.
 * a_cache[1..max_computed] stores the sequence values,
 * row[1..max_computed] stores the current last row of T. */
static seq_t *a_cache = NULL;          /* sequence cache */
static size_t a_cache_cap = 0;          /* current capacity of a_cache */
static seq_t *row = NULL;              /* current row of T */
static idx_t max_computed = 0;          /* highest n for which row and a_cache are valid */

/* Computes A131076: the sum of the nth row of the triangular array T. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1, but guard against 0 */

    /* Expand the cache and the row up to n if necessary. */
    for (idx_t new_n = max_computed + 1; new_n <= n; ++new_n) {
        /* Ensure cache capacity. */
        if ((size_t)new_n + 1 > a_cache_cap) {
            size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
            while (new_cap <= (size_t)new_n + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failed */
            a_cache = tmp;
            a_cache_cap = new_cap;
        }

        /* Allocate a new row of length new_n. */
        size_t new_len = (size_t)new_n + 1;   /* indices 1..new_n */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_row) abort();              /* allocation failed */

        /* First element of the row. */
        seq_t first = (((new_n - 1ULL) % 8ULL) < 4ULL) ? (seq_t)1 : (seq_t)0;
        new_row[1] = first;
        seq_t sum = first;

        /* Compute the rest of the row using the previous row. */
        if (max_computed == 0) {           /* first row, nothing to add */
            /* nothing more to do */
        } else {
            for (idx_t k = 2; k <= new_n; ++k) {
                seq_t val = row[k - 1] + new_row[k - 1];
                new_row[k] = val;
                sum += val;
            }
        }

        /* Store the computed sequence value. */
        a_cache[new_n] = sum;

        /* Free the old row and adopt the new row. */
        if (row) free(row);
        row = new_row;
        max_computed = new_n;
    }

    return a_cache[n];
}
