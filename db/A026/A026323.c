/*
 * A026323  Irregular triangular array T read by rows: T(0,0) = 1, T(0,1) =
 *          T(0,2) = 0; T(1,0) = T(1,1) = T(1,2) = 1, T(1,3) = 0; for n >= 2,
 *          T(n,0) = 1, T(n,1) = T(n-1,0) + T(n-1,1), T(n,k) = T(n-1,k-2) +
 *          T(n-1,k-1) + T(n-1,k) for k = 2,3,...,n+1 and T(n,n+2) = T(n-1,n)
 *          + T(n-1,n+1).
 * https://oeis.org/A026323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used:
 *
 *   T(0,0)=1,  T(0,1)=T(0,2)=0
 *   T(1,0)=T(1,1)=T(1,2)=1,  T(1,3)=0
 *   For n≥2:
 *     T(n,0)=1
 *     T(n,1)=T(n-1,0)+T(n-1,1)
 *     T(n,k)=T(n-1,k-2)+T(n-1,k-1)+T(n-1,k)   for 2≤k≤n+1
 *     T(n,n+2)=T(n-1,n)+T(n-1,n+1)
 *
 * The sequence a(n) is the triangular array T read by rows.
 * a(1)=T(0,0), a(2)=T(0,1), …
 *
 * The implementation builds rows iteratively, stores all elements in a
 * contiguous dynamic array, and memoises the results so that any call to
 * a(idx_t n) returns a(n) in O(1) after the necessary rows have been
 * generated. */

#include <stdlib.h>
#include <stddef.h>

/* Global state for memoisation */
static seq_t *seq_arr = NULL;      /* flattened terms of the triangle */
static size_t seq_cap = 0;
static size_t seq_len = 0;

static seq_t *row_prev = NULL;     /* previous row of the triangle */
static size_t row_prev_len = 0;
static idx_t current_row = (idx_t)-1;   /* -1 indicates not initialised */

/* Grow the global sequence array to hold at least 'need' more elements. */
static void grow_seq_array(size_t need)
{
    if (seq_len + need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < seq_len + need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* Initialise the first row (n = 0) of the triangle and seed the sequence. */
static void init_first_row(void)
{
    row_prev_len = 3;
    row_prev = (seq_t *)malloc(row_prev_len * sizeof(seq_t));
    row_prev[0] = 1;
    row_prev[1] = 0;
    row_prev[2] = 0;

    grow_seq_array(row_prev_len);
    for (size_t i = 0; i < row_prev_len; ++i)
        seq_arr[seq_len++] = row_prev[i];

    current_row = 0;
}

/* Generate rows until the sequence contains at least 'n' terms. */
static void ensure_seq_upto(idx_t n)
{
    if (seq_len < (size_t)n) {
        if (current_row == (idx_t)-1)
            init_first_row();

        while (seq_len < (size_t)n) {
            /* Build the next row: row number = current_row + 1 */
            idx_t next_row = current_row + 1;
            size_t next_len = (size_t)(next_row + 3);   /* length = n + 3 */

            seq_t *row_next = (seq_t *)malloc(next_len * sizeof(seq_t));

            /* k = 0 */
            row_next[0] = 1;

            /* k = 1 */
            row_next[1] = row_prev[0] + row_prev[1];

            /* 2 ≤ k ≤ next_row + 1 */
            for (idx_t k = 2; k <= next_row + 1; ++k) {
                size_t idx = (size_t)k;
                seq_t sum = row_prev[idx - 2] + row_prev[idx - 1] + row_prev[idx];
                row_next[idx] = sum;
            }

            /* Last element k = next_row + 2 */
            size_t last = (size_t)(next_row + 2);
            row_next[last] = row_prev[next_row] + row_prev[next_row + 1];

            /* Append to global sequence */
            grow_seq_array(next_len);
            for (size_t i = 0; i < next_len; ++i)
                seq_arr[seq_len++] = row_next[i];

            /* Prepare for next iteration */
            free(row_prev);
            row_prev = row_next;
            row_prev_len = next_len;
            current_row = next_row;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_seq_upto(n);
    return seq_arr[n - 1];
}
