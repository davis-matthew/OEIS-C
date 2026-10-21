/*
 * A061312  Triangle T[n,m]: T[n,-1] = 0; T[0,0] = 0; T[n,0] = n*n!; T[n,m] =
 *          T[n,m-1] - T[n-1,m-1].
 * https://oeis.org/A061312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 594, big any n
 * Helpers:   index_to_row_col, seq_alloc_new_array, seq_alloc_new_ptr_array,
 *            seq_free_array, seq_free_ptr_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061312 – Triangle T[n,m] : T[n,0] = n * n!,  T[n,m] = T[n,m-1] – T[n-1,m-1],
   read row‑wise.  a(n) = the n‑th entry of the triangle.  The code
   builds the triangle on demand and caches it for subsequent calls.
   ---------------------------------------------------------------
   Requires external types:
     idx_t – unsigned integer type for indices (uint64_t)
     seq_t – unsigned integer or arbitrary‑precision integer type
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/index_to_row_col.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_alloc_new_ptr_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_free_ptr_array.h"

#ifdef __cplusplus

#else

#endif

/* Resize an array of seq_t elements.  Copies old contents, frees old. */
static seq_t* seq_resize_array(seq_t* old, size_t old_len, size_t new_len)
{
    seq_t* newptr = seq_alloc_new_array(new_len);
    if (!newptr) abort();                 /* allocation failure */
    for (size_t i = 0; i < old_len; ++i) newptr[i] = old[i];
    seq_free_array(old);
    return newptr;
}

/* Resize an array of seq_t* pointers. */
static seq_t** seq_resize_ptr_array(seq_t** old, size_t old_len, size_t new_len)
{
    seq_t** newptr = seq_alloc_new_ptr_array(new_len);
    if (!newptr) abort();
    for (size_t i = 0; i < old_len; ++i) newptr[i] = old[i];
    seq_free_ptr_array(old);
    return newptr;
}

/* ---------------------------------------------------------------
   Global cached data: factorials and triangle rows
   ---------------------------------------------------------------
*/
static seq_t* fact = NULL;          /* fact[i] = i! */
static size_t fact_size = 0;        /* allocated size of fact */

/* rows[i] points to array of T[i, m] for m = 0..i */
static seq_t** rows = NULL;
static size_t rows_len = 0;         /* allocated number of rows */

/* ------------------------------------------------------------------
   Helper: ensure factorials up to n are available
   ------------------------------------------------------------------ */
static void ensure_factorials(idx_t n)
{
    if (fact_size > (size_t)n) return;
    size_t new_size = (size_t)n + 1;
    fact = seq_resize_array(fact, fact_size, new_size);
    if (fact_size == 0) {
        fact[0] = (seq_t)1;
        fact_size = 1;
    }
    for (size_t i = fact_size; i <= (size_t)n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_size = new_size;
}

/* ------------------------------------------------------------------
   Helper: ensure triangle rows up to n are computed and cached
   ------------------------------------------------------------------ */
static void ensure_rows(idx_t n)
{
    if (rows_len > (size_t)n) return;
    size_t new_len = (size_t)n + 1;
    rows = seq_resize_ptr_array(rows, rows_len, new_len);
    for (size_t r = rows_len; r <= (size_t)n; ++r) {
        size_t len = r + 1;
        seq_t* curr = seq_alloc_new_array(len);
        if (!curr) abort();
        /* T[r,0] = r * r! */
        curr[0] = (seq_t)r * fact[r];
        /* T[r,m] = T[r,m-1] – T[r-1,m-1]  for m ≥ 1 */
        for (size_t m = 1; m <= r; ++m) {
            curr[m] = curr[m-1] - rows[r-1][m-1];
        }
        rows[r] = curr;
    }
    rows_len = new_len;
}

/* ---------------------------------------------------------------
   Main function required by the build system
   --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_row_col(n, &row, &col);
    if (fact_size <= (size_t)row) ensure_factorials(row);
    if (rows_len <= (size_t)row)   ensure_rows(row);
    return rows[row][col];
}
