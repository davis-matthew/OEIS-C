/*
 * A014726  Squares of odd elements in Pascal triangle.
 * https://oeis.org/A014726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A014421(n)^2
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache of computed terms */
static seq_t *cache      = NULL;   /* array of a(k) */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* state for incremental generation */
static idx_t   current_row = 0;     /* last fully processed row index */
static seq_t * prev_row    = NULL;  /* array of binomial coefficients of current_row */
static size_t prev_len    = 0;      /* length of prev_row (== current_row+1) */

/* Append a value to the cache, reallocating if necessary */
static void cache_append(seq_t val)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Initialise the very first row of Pascal's triangle. */
static void init_first_row(void)
{
    if (prev_row) return;          /* already initialised */

    prev_row = (seq_t *)malloc(sizeof(seq_t));
    if (!prev_row) exit(1);
    prev_row[0] = (seq_t)1;        /* C(0,0)=1 */
    prev_len   = 1;
    current_row = 0;

    /* first odd element is 1 */
    cache_append(prev_row[0]);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have at least the first term ready */
    if (cache_size == 0) init_first_row();

    /* Generate rows until we have computed term a(n) */
    while (cache_size <= n) {
        /* Compute next row of Pascal's triangle */
        idx_t      r       = current_row + 1;
        size_t     next_len = prev_len + 1;
        seq_t *    new_row  = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!new_row) exit(1);

        new_row[0] = (seq_t)1;
        new_row[next_len - 1] = (seq_t)1;

        for (size_t k = 1; k + 1 < next_len; ++k) {
            new_row[k] = prev_row[k - 1] + prev_row[k];
        }

        /* Store squares of the odd elements of this row */
        for (size_t k = 0; k < next_len; ++k) {
            seq_t val = new_row[k];
            if ((val & (seq_t)1) != (seq_t)0) {           /* odd check */
                seq_t sq = val;
                sq *= val;                               /* square it */
                cache_append(sq);
            }
        }

        /* Advance to the next row */
        free(prev_row);
        prev_row = new_row;
        prev_len = next_len;
        current_row = r;
    }

    return cache[n];
}
