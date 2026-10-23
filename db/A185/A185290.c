/*
 * A185290  Lexicographically least permutation of the integers in a triangle
 *          satisfying T(n,k) + T(n+1,k) <= T(n+1,k+1).
 * https://oeis.org/A185290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 104, u32 n <= 434, u64 n <= 1829,
 *            u128 n <= 7625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A185290 – Lexicographically least permutation of integers in a triangle
 * satisfying T(n,k) + T(n+1,k) <= T(n+1,k+1). 
 * This implementation builds the triangle row by row.  For each position (n,k) 
 * it chooses the smallest unused integer that is at least 
 * T(n-1,k-1) + T(n,k-1) (this is the only lower bound needed).  
 * The sequence is stored in linear order so that a(n) can be returned directly. 
 */

#include <stdlib.h>
#include <stdbool.h>

/* Check if a value is already used. */
static bool used_contains(const seq_t *used, size_t used_len, const seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val)
            return true;
    return false;
}

/* Append a value to a dynamic array, expanding if necessary. */
static void push_back(seq_t **arr, size_t *len, size_t *cap, const seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < *len; ++i)
            new_arr[i] = (*arr)[i];
        delete[] *arr;
#else
        new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
#endif
        *arr = new_arr;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Persistent state across calls */
    static seq_t *data = NULL;
    static size_t data_len = 0, data_cap = 0;
    static seq_t *used = NULL;
    static size_t used_len = 0, used_cap = 0;
    static idx_t rows = 0;          /* number of rows currently constructed */
    static seq_t min_unused = 1;    /* smallest unused integer */

    /* Grow the triangle until it contains at least n elements */
    while (data_len < n) {
        /* Construct the next row */
        ++rows;
        size_t row_len = (size_t)rows;          /* how many numbers in this row */
        size_t row_start = rows * (rows - 1) / 2; /* linear index of first entry of this row */
        size_t prev_start = row_start - (rows - 1);  /* start position of previous row */

        for (size_t k = 1; k <= row_len; ++k) {
            seq_t val;

            if (k == 1) {  /* first element of the row – no lower bound */
                val = min_unused;
            } else {
                /* lower bound from the element directly above-left and left neighbor */
                seq_t left_above = data[prev_start + (k - 2)];
                seq_t left_cur   = data[row_start + (k - 2)];
                seq_t lower      = left_above + left_cur;
                val = lower;
                /* find the smallest unused integer >= lower */
                while (used_contains(used, used_len, val))
                    val = val + 1;
            }

            /* Store the new value */
            push_back(&data, &data_len, &data_cap, val);
            push_back(&used, &used_len, &used_cap, val);

            /* Update the global minimum unused integer if necessary */
            if (val == min_unused) {
                do {
                    min_unused = min_unused + 1;
                } while (used_contains(used, used_len, min_unused));
            }
        }
    }

    /* Return the requested term (1‑based index). */
    return data[n - 1];
}
