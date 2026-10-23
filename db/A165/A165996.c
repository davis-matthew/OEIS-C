/*
 * A165996  Triangle read by rows: T(n,0) = (n+1)^2, T(n,k) = T(n,k-1) +
 *          T(n-1,k) for 0 < k < n, and T(n,n) = T(n,n-1).
 * https://oeis.org/A165996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   T(n,0) = (n+1)^2
   T(n,k) = T(n,k-1) + T(n-1,k)   for 0 < k < n
   T(n,n) = T(n,n-1)
   a(n) is the flattened sequence of T(n,k), offset 0. */
#include <stdlib.h>

/* Static tables for memoization. */
static seq_t *seq_data = NULL;   /* flat triangle values */
static idx_t seq_len  = 0;       /* number of values stored */
static size_t seq_cap = 0;       /* allocated capacity of seq_data */

static seq_t *prev_row = NULL;   /* last computed row (size == rows_done) */
static idx_t rows_done = 0;      /* number of rows already generated */

/* Ensure that seq_data contains at least 0 … n elements. */
static void ensure_upto(idx_t n)
{
    while (seq_len <= n) {
        /* Size of the next row to generate. */
        idx_t i = rows_done;

        /* Grow the flattened array if needed. */
        size_t needed = (size_t)seq_len + (size_t)(i + 1);
        if (needed > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_data, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* out of memory */
            seq_data = tmp;
            seq_cap = new_cap;
        }

        /* Allocate a working row of size i+1. */
        seq_t *cur_row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!cur_row) abort();

        /* T(i,0) = (i+1)^2. */
        seq_t inc = (seq_t)(i + 1);
        cur_row[0] = inc * inc;
        seq_data[seq_len++] = cur_row[0];

        /* Compute the inner entries T(i,k) for 1 <= k < i. */
        for (idx_t k = 1; k < i; ++k) {
            cur_row[k] = cur_row[k - 1] + prev_row[k];
            seq_data[seq_len++] = cur_row[k];
        }

        /* T(i,i) = T(i,i-1). */
        if (i > 0) {
            cur_row[i] = cur_row[i - 1];
            seq_data[seq_len++] = cur_row[i];
        }

        /* Replace the previous row with the current one. */
        free(prev_row);
        prev_row = cur_row;
        rows_done = i + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return seq_data[n];
}
