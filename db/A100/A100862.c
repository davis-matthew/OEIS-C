/*
 * A100862  Triangle read by rows: T(n,k) is the number of k-matchings of the
 *          corona K'(n) of the complete graph K(n) and the complete graph
 *          K(1); in other words, K'(n) is the graph constructed from K(n) by
 *          adding for each vertex v a new vertex v' and the edge vv'.
 * https://oeis.org/A100862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k) + (n-1)·T(n-2,k-1) + T(n-1,k-1), T(0,0)=1.
 *            a(n) = T(r,c) where r·(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *            c=n−r(r+1)/2
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   mul_div2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_div2.h"

/* Find the greatest row index r such that r·(r+1)/2 ≤ n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (mul_div2(high, high + 1) <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mul_div2(mid, mid + 1) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Storage for all pre‑computed triangle entries. */
static seq_t *data = NULL;         /* flattened array of size total_entries */
static idx_t data_capacity = 0;    /* allocated size in elements */
static idx_t current_max_row = 0;  /* highest row that has been computed */

/* Ensure the buffer can hold all entries up to row r. */
static void grow_buffer(idx_t r)
{
    idx_t required = (r + 1) * (r + 2) / 2;     /* total elements up to row r */
    if (required <= data_capacity) return;

    idx_t new_capacity = data_capacity ? data_capacity : 1;
    while (new_capacity < required) new_capacity <<= 1;

    seq_t *tmp = (seq_t *)realloc(data, new_capacity * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failure */
    data        = tmp;
    data_capacity = new_capacity;
}

/* Compute all rows up to and including row r. */
static void compute_up_to_row(idx_t r)
{
    grow_buffer(r);
    for (idx_t i = current_max_row + 1; i <= r; ++i) {
        idx_t row_start = i * (i + 1) / 2;
        idx_t prev_row_start = (i - 1) * i / 2;                 /* start of row i-1 */
        idx_t prev2_row_start = (i >= 2) ? (i - 2) * (i - 1) / 2 : 0; /* start of row i-2 */

        for (idx_t c = 0; c <= i; ++c) {
            seq_t val = 0;

            /* T(i-1, c) */
            if (c <= i - 1)
                val += data[prev_row_start + c];

            /* (i-1)·T(i-2, c-1) */
            if (i >= 2 && c >= 1 && c <= i - 1)
                val += (seq_t)(i - 1) * data[prev2_row_start + (c - 1)];

            /* T(i-1, c-1) */
            if (c >= 1)
                val += data[prev_row_start + (c - 1)];

            data[row_start + c] = val;
        }
    }
    current_max_row = r;
}

/* Public function that returns the n‑th term of the flattened sequence. */
seq_t a(idx_t n)
{
    /* First call: initialise the storage with T(0,0)=1. */
    if (!data) {
        data = (seq_t *)malloc(sizeof(seq_t));
        if (!data) abort();
        data[0] = (seq_t)1;
        data_capacity = 1;
        current_max_row = 0;
    }

    /* Determine which row and column correspond to index n. */
    idx_t row = find_row(n);
    idx_t row_start = row * (row + 1) / 2;
    idx_t col = n - row_start;

    /* Compute missing rows up to 'row'. */
    if (row > current_max_row)
        compute_up_to_row(row);

    return data[row_start + col];
}
