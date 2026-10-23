/*
 * A157150  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 4, read by rows.
 * https://oeis.org/A157150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k,4) = (4*(n-k)+1)*T(n-1,k-1,4) + (4*k+1)*T(n-1,k,4) +
 *            4*k*(n-k)*T(n-2,k-1,4) with T(n,0,4)=T(n,n,4)=1. The function
 *            a(n) returns the flattened triangle (a(n))
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 47, u64 n <= 124,
 *            u128 n <= 334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

/*---------------------------------------------------------------------*
 *  Forward declarations for memory helpers used when compiling as C *
 *---------------------------------------------------------------------*/
#ifndef __cplusplus
/* The build system provides these when required. They are only used
   when compiled as C and seq_t is a fundamental integer type. */
extern void *seq_alloc_array(size_t);      /* allocate array of seq_t */
extern void  seq_free_array(seq_t*);       /* free array allocated with seq_alloc_array */
extern seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); /* realloc a seq_t array */
#endif

static idx_t rows_computed = 0;  /* number of rows already computed */

/*---------------------------------------------------------------------*
 *  Structures for C++ compilation: std::vector for storage           *
 *---------------------------------------------------------------------*/
#ifdef __cplusplus
static std::vector<seq_t> seq_values;      /* flattened triangle */
static std::vector<seq_t> prev_row;       /* row r-1 */
static std::vector<seq_t> prev_prev_row;  /* row r-2 */

static void
compute_row(idx_t r)
{
    idx_t new_len = r + 1;
    std::vector<seq_t> new_row(new_len);

    for (idx_t k = 0; k < new_len; ++k) {
        if (k == 0 || k == r) {
            new_row[k] = 1;
        } else {
            seq_t mult1 = 4 * (seq_t)(r - k) + 1;
            seq_t term1  = mult1 * prev_row[k - 1];

            seq_t mult2 = 4 * (seq_t)k + 1;
            seq_t term2  = mult2 * prev_row[k];

            seq_t mult3 = 4 * (seq_t)(k * (r - k));
            seq_t term3  = mult3 * prev_prev_row[k - 1];

            new_row[k] = term1 + term2 + term3;
        }
    }

    /* append to flattened sequence */
    seq_values.insert(seq_values.end(), new_row.begin(), new_row.end());

    /* rotate rows: prev_prev_row <- prev_row, prev_row <- new_row */
    prev_prev_row.swap(prev_row);
    prev_row.swap(new_row);
}
#else
/*---------------------------------------------------------------------*
 *  Structures for C compilation: raw pointers for storage             *
 *---------------------------------------------------------------------*/
static seq_t *seq_values = NULL;      /* flattened triangle */
static size_t seq_cap   = 0;          /* capacity of seq_values */
static size_t seq_len   = 0;          /* number of values stored */

static seq_t *prev_row      = NULL;   /* row r-1 */
static idx_t  prev_row_len  = 0;
static seq_t *prev_prev_row = NULL;  /* row r-2 */
static idx_t  prev_prev_row_len = 0;

/* ensure seq_values has at least new_cap elements */
static void
ensure_seq_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t ne = seq_cap ? seq_cap : 4;
    while (ne < new_cap) ne <<= 1;
    seq_values = (seq_t *)realloc(seq_values, ne * sizeof(seq_t));
    seq_cap = ne;
}

/* compute triangle row with index r (0‑based) */
static void
compute_row(idx_t r)
{
    idx_t new_len = r + 1;
    seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_row) return;  /* abort on OOM; result is undefined */

    for (idx_t k = 0; k < new_len; ++k) {
        if (k == 0 || k == r) {
            new_row[k] = 1;
        } else {
            seq_t term1 = (4 * (seq_t)(r - k) + 1) * prev_row[k - 1];
            seq_t term2 = (4 * (seq_t)k + 1) * prev_row[k];
            seq_t term3 = (4 * (seq_t)(k * (r - k))) * prev_prev_row[k - 1];
            new_row[k] = term1 + term2 + term3;
        }
    }

    /* append to flattened sequence */
    ensure_seq_capacity(seq_len + new_len);
    for (idx_t k = 0; k < new_len; ++k) {
        seq_values[seq_len++] = new_row[k];
    }

    /* free old prev_prev_row and shift row pointers */
    if (prev_prev_row) free(prev_prev_row);
    prev_prev_row = prev_row;
    prev_prev_row_len = prev_row_len;
    prev_row = new_row;
    prev_row_len = new_len;
}
#endif

/*---------------------------------------------------------------------*
 *  Public routine required by the build system                        *
 *---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
#ifdef __cplusplus
    while (seq_values.size() <= (size_t)n) {
        compute_row(rows_computed);
        ++rows_computed;
    }
    return seq_values[(size_t)n];
#else
    while (seq_len <= (size_t)n) {
        compute_row(rows_computed);
        ++rows_computed;
    }
    return seq_values[(size_t)n];
#endif
}
