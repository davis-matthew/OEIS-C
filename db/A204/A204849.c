/*
 * A204849  A Motzkin triangle by rows.
 * https://oeis.org/A204849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   This function returns the terms of OEIS A204849: the flattened Motzkin
   triangle read row‑by‑row (row0 = [1], row1 = [1,1], row2 = [2,1,1], …).
   The row n of the triangle is the first row of Mⁿ, where M is the 
   infinite production matrix

          1 1 0 0 0 ...
          1 0 1 0 0 ...
          1 1 0 1 0 ...
          1 1 1 0 1 0 ...
          1 1 1 1 0 1 0 ...
          1 1 1 1 1 0 1 0 ...
          ⋱

   The function builds the rows successively using this matrix until it
   reaches the row containing the requested index.  No arithmetic
   beyond the standard integer operations is used. */
  
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
static inline seq_t *alloc_row(size_t len)
{
    seq_t *ptr = new seq_t[len];
    for (size_t i = 0; i < len; ++i) ptr[i] = (seq_t)0;
    return ptr;
}
static inline void free_row(seq_t *ptr)
{
    delete [] ptr;
}
#else
static inline seq_t *alloc_row(size_t len)
{
    return (seq_t *)calloc(len, sizeof(seq_t));
}
static inline void free_row(seq_t *ptr)
{
    free(ptr);
}
#endif

/* Return the largest r with r*(r+1)/2 <= n.  Uses a doubling
   search followed by a binary search.  The product is cast to
   128‑bit to avoid overflow. */
static inline uint64_t find_row(uint64_t n)
{
    uint64_t low = 0, high = 1;
    while (((unsigned __int128)high * (high + 1) / 2) <= n) high <<= 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    uint64_t n_val = (uint64_t)n;
    uint64_t r          = find_row(n_val);                     /* row number */
    uint64_t sum_before = (unsigned __int128)r * (r + 1) / 2;    /* start index of row r */
    uint64_t p          = n_val - sum_before;                  /* column index in row r */

    /* Allocate buffers for two successive rows.  max_len = r+1 is the
       largest row length that will appear. */
    size_t max_len = (size_t)(r + 1);
    seq_t *prev = alloc_row(max_len);
    seq_t *next = alloc_row(max_len);

    /* Initial row (row 0) */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r.  For each i in the previous row we add its
       value to all j <= i+1 in the next row, except for the diagonal
       cell M[i][i] when i > 0 (for i == 0 the diagonal is 1). */
    for (size_t row = 0; row < r; ++row) {
        size_t len_prev = row + 1;       /* length of prev row   */
        size_t len_next = len_prev + 1;  /* length of next row   */

        /* Zero the part of next that will be written. */
        for (size_t j = 0; j < len_next; ++j) next[j] = (seq_t)0;

        for (size_t i = 0; i < len_prev; ++i) {
            seq_t val = prev[i];
            for (size_t j = 0; j <= i + 1; ++j) {
                /* Skip the diagonal entry M[i][i] if i > 0. */
                if (i > 0 && j == i) continue;
                next[j] = next[j] + val;
            }
        }

        /* Swap buffers for the next iteration. */
        seq_t *tmp  = prev;
        prev = next;
        next = tmp;
    }

    /* prev now holds row r. Retrieve the desired element. */
    seq_t result = prev[(size_t)p];

    /* Free temporary buffers. */
    free_row(next);   /* next points to the old prev */
    free_row(prev);

    return result;
}
