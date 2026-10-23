/*
 * A121634  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n and having k 2-cell columns starting at level 0 (n >= 1;
 *          0 <= k <= n-1).
 * https://oeis.org/A121634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle T(n,k) satisfies the recurrence P(n,t) = (t + n -
 *            2) * ( (n-2)! + P(n-1,t) ), with P(1,t)=1 and P(2,t)=1+t. The
 *            sequence a(n) is the flattened list of the rows of T
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

static seq_t **rows_arr = NULL;      /* array of row pointers, 1‑indexed */
static size_t rows_alloc = 0;        /* current capacity of rows_arr (index max+1) */
static size_t rows_count = 0;        /* highest row computed so far */
static seq_t fact = (seq_t)1;        /* factorial of (current_row-2) */

/* ------------------------------------------------------------------ */
/* Allocation helpers that work in both C and C++                       */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static inline seq_t* alloc_seq_array(size_t len)
{ return new seq_t[len]; }

static inline seq_t* copy_seq_array(const seq_t* src, size_t len)
{
    seq_t* dst = new seq_t[len];
    for (size_t i = 0; i < len; ++i) dst[i] = src[i];
    return dst;
}

static inline seq_t** alloc_row_ptr_array(size_t len)
{ return new seq_t*[len]; }
#else
static inline seq_t* alloc_seq_array(size_t len)
{ return calloc(len, sizeof(seq_t)); }

static inline seq_t** alloc_row_ptr_array(size_t len)
{ return calloc(len, sizeof(seq_t*)); }
#endif

/* ------------------------------------------------------------------ */
/* Ensure all rows up to 'target' are computed                          */
/* ------------------------------------------------------------------ */
static void ensure_rows(idx_t target)
{
    /* ---------- lazy initialization ---------- */
    if (rows_arr == NULL) {
        rows_alloc = 3;                     /* handle indices 0,1,2 */
        rows_arr = alloc_row_ptr_array(rows_alloc);

        /* row 1: [1] */
        rows_arr[1] = alloc_seq_array(1);
        rows_arr[1][0] = (seq_t)1;

        /* row 2: [1,1] */
        rows_arr[2] = alloc_seq_array(2);
        rows_arr[2][0] = (seq_t)1;
        rows_arr[2][1] = (seq_t)1;

        rows_count = 2;
        /* fact already 1 --> (0)! for next row (n=3) */
    }

    /* Nothing to do if already computed */
    if (rows_count >= (size_t)target)
        return;

    /* ---------- enlarge row pointer array if needed ---------- */
    if (rows_alloc <= (size_t)target) {
        size_t new_alloc = (size_t)target + 1;
#ifdef __cplusplus
        seq_t** new_arr = alloc_row_ptr_array(new_alloc);
        for (size_t i = 0; i < rows_alloc; ++i) new_arr[i] = rows_arr[i];
        delete[] rows_arr;
        rows_arr = new_arr;
#else
        seq_t** new_arr = realloc(rows_arr, new_alloc * sizeof(seq_t*));
        rows_arr = new_arr;
#endif
        rows_alloc = new_alloc;
    }

    /* ---------- compute missing rows ---------- */
    for (size_t r = rows_count + 1; r <= (size_t)target; ++r) {
        if (r == 1) continue;   /* already handled */
        if (r == 2) continue;   /* already handled */

        size_t prev_len = r - 1;
        seq_t* prev = rows_arr[r - 1];
        size_t new_len = r;
        seq_t* newrow = alloc_seq_array(new_len);
        /* zero-initialize (alloc_seq_array does this in C, we set explicitly in C++) */
#ifndef __cplusplus
        /* already zeroed by calloc */
#endif
        /* multiply Pprev + fact by (t + b) where b = r-2 */
        seq_t base = (seq_t)(r - 2);   /* coefficient of t in (t + b) */
        seq_t c    = fact;           /* (n-2)! */

        for (size_t j = 0; j < prev_len; ++j) {
            seq_t q = prev[j];
            newrow[j]   += q * base; /* b * q * t^j */
            newrow[j+1] += q;        /* q * t^{j+1} */
        }
        newrow[0] += c * base; /* c*b */
        newrow[1] += c;        /* c*t */

        rows_arr[r] = newrow;
        fact *= (seq_t)(r - 1);   /* prepare fact = (r-1)! for next iteration */
    }

    rows_count = target;
}

/* ------------------------------------------------------------------ */
/* Main entry: compute the nth flattened value of the triangle        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is 1‑indexed; undefined for n=0 */
        return (seq_t)0;

    /* find row r containing position n:
       r satisfies r*(r+1)/2 >= n
    */
    idx_t r = (idx_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* adjust for possible rounding errors */
    while ((idx_t)((r * (r + 1)) / 2) < n) ++r;
    while ((idx_t)(((r - 1) * r) / 2) >= n) --r;

    ensure_rows(r);

    idx_t prev_sum = (idx_t)((r * (r - 1)) / 2);          /* terms before row r */
    idx_t col = n - prev_sum - 1;                         /* column index (0‑based) */

    return rows_arr[(size_t)r][(size_t)col];
}
