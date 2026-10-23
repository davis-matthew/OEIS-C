/*
 * A185081  Triangle T(n,k), read by rows, given by (0, 1, 1, -1, 0, 0, 0, 0,
 *          0, 0, 0, ...) DELTA (1, 1, -1, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A185081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   alloc_seq_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A185081 – Triangle T(n,k) with recurrence
 *  T(n,k)=T(n‑1,k)+T(n‑1,k‑1)+T(n‑2,k)+T(n‑2,k‑1)+T(n‑2,k‑2)
 *  with T(0,0)=1, T(1,0)=0, T(1,1)=1, T(2,0)=0, T(2,1)=1, T(2,2)=2.
 *  a(n) is the row‑major element of the triangle.  */

/* Standard helpers – use the big‑integer allocator if compiled as C++. */
#if defined(__cplusplus)
#include <new>
#endif
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_n.h"

/* Helper: allocate an array of seq_t of length n. */

/* Helper: realloc array of seq_t * (for row‑pointers). */
static seq_t **
realloc_rows(seq_t **ptr, size_t new_cap)
{
#if defined(__cplusplus)
    return (seq_t **)realloc(ptr, new_cap * sizeof(seq_t *));
#else
    return (seq_t **)realloc(ptr, new_cap * sizeof(seq_t *));
#endif
}

/* Persistent storage for computed rows. */
static seq_t **T_rows      = NULL;   /* array of pointers to each row */
static size_t   T_rows_cap = 0;       /* allocated capacity for row pointers */
static idx_t    T_rows_cnt = 0;       /* number of rows that have been computed */

/* Ensure rows 0..r are computed, storing them in T_rows[0..r]. */
static void
ensure_rows(idx_t r)
{
    /* Grow the row‑pointer array if necessary. */
    if (T_rows_cap <= (size_t)r) {
        size_t new_cap = (size_t)r + 1;
        seq_t **new_rows = realloc_rows(T_rows, new_cap);
        if (!new_rows) exit(1);          /* memory allocation failed */
        T_rows = new_rows;
        for (size_t i = T_rows_cap; i < new_cap; ++i)
            T_rows[i] = NULL;
        T_rows_cap = new_cap;
    }

    /* Compute any missing rows. */
    for (idx_t i = T_rows_cnt; i <= r; ++i) {
        /* Allocate space for row i (i+1 elements). */
        seq_t *row = alloc_seq_n((size_t)i + 1);
        if (!row) exit(1);
        T_rows[i] = row;

        /* Initialise the row from the recurrence. */
        if (i == 0) {                      /* Row 0 : 1 */
            row[0] = (seq_t)1;
        } else if (i == 1) {               /* Row 1 : 0,1 */
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
        } else if (i == 2) {               /* Row 2 : 0,1,2 */
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
            row[2] = (seq_t)2;
        } else {                           /* i >= 3 */
            seq_t *prev  = T_rows[(size_t)i - 1];
            seq_t *prev2 = T_rows[(size_t)i - 2];
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = (seq_t)0;
                /* T(n‑1,k) */
                if (k <= i - 1) val += prev[k];
                /* T(n‑1,k‑1) */
                if (k >= 1 && k - 1 <= i - 1) val += prev[k - 1];
                /* T(n‑2,k) */
                if (k <= i - 2) val += prev2[k];
                /* T(n‑2,k‑1) */
                if (k >= 1 && k - 1 <= i - 2) val += prev2[k - 1];
                /* T(n‑2,k‑2) */
                if (k >= 2 && k - 2 <= i - 2) val += prev2[k - 2];
                row[k] = val;
            }
        }
    }
    T_rows_cnt = r + 1;
}

/* Find row r and starting index of that row in the row‑major sequence. */
static void
row_and_start(idx_t n, idx_t *r, idx_t *start)
{
    idx_t s = 0;
    for (idx_t rr = 0; ; ++rr) {
        if (n < s + rr + 1) {            /* n is inside row rr */
            *r     = rr;
            *start = s;
            return;
        }
        s += rr + 1;                      /* next row starts here */
    }
}

/* Public entry point. */
seq_t
a(idx_t n)
{
    idx_t r, start;
    row_and_start(n, &r, &start);
    ensure_rows(r);
    idx_t c = n - start;                     /* column within row r */
    return T_rows[(size_t)r][c];
}
