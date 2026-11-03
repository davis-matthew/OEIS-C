/*
 * A084606  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1+2x+2x^2)^n.
 * https://oeis.org/A084606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^(n - floor(sqrt(n))^2) in (1 + 2x +
 *            2x^2)^(floor(sqrt(n)))
 * Range:     u8 n <= 28, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    /* Static state: cached sequence values and rows of the triangular array */
    static seq_t *data   = NULL;   /* flat array of the sequence values */
    static size_t data_size = 0;   /* number of terms currently stored */
    static size_t data_cap  = 0;   /* allocated capacity of data */

    static seq_t *prev_row = NULL; /* coefficients of the last computed row r-1 */
    static size_t prev_len = 0;    /* length of prev_row (2*(r-1)+1) */
    static idx_t  last_row  = 0;   /* number of the last computed row */

    static bool initialized = false;   /* first‑time initialisation flag */

    /* Initialise for the first call: row 0 contains only the coefficient 1. */
    if (!initialized) {
        ensure_capacity(1, &data_cap, OEISC_VPP(&data), sizeof(seq_t));
        data[0] = 1;
        data_size = 1;

        prev_row = mem_new(seq_t, 1);
        prev_row[0] = 1;
        prev_len = 1;
        last_row = 0;

        initialized = true;
    }

    /* If the requested index is already cached, return it immediately. */
    if (n < data_size)
        return data[n];

    /* Grow the triangle row by row until the cache covers index n. */
    while (data_size <= n) {
        idx_t r = last_row + 1;                 /* next row index   */
        size_t curr_len = 2 * r + 1;            /* number of terms in row r */

        /* Allocate a temporary array for the current row. */
        seq_t *curr = mem_new(seq_t, curr_len);

        /* Compute each coefficient of row r using the recurrence
         * a(r,k) = a(r-1,k) + 2*a(r-1,k-1) + 2*a(r-1,k-2).        */
        for (size_t i = 0; i < curr_len; ++i) {
            seq_t val = 0;
            if (i < prev_len)
                val += prev_row[i];
            if (i >= 1 && (i - 1) < prev_len)
                val += 2 * prev_row[i - 1];
            if (i >= 2 && (i - 2) < prev_len)
                val += 2 * prev_row[i - 2];
            curr[i] = val;
        }

        /* Append the newly computed row to the flat sequence array. */
        ensure_capacity(data_size + curr_len, &data_cap, OEISC_VPP(&data), sizeof(seq_t));
        for (size_t i = 0; i < curr_len; ++i)
            data[data_size++] = curr[i];

        /* Prepare for the next iteration: the new row becomes the previous one. */
        mem_free(prev_row);
        prev_row = curr;
        prev_len = curr_len;
        last_row = r;
    }

    return data[n];
}
