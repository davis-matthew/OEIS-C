/*
 * A201947  Triangle T(n,k), read by rows, given by (1,1,-1,0,0,0,0,0,0,0,...)
 *          DELTA (1,-1,1,0,0,0,0,0,0,...) where DELTA is the operator defined
 *          in A084938.
 * https://oeis.org/A201947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 36, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th entry of the flattened triangle T(n,k).
   Triangle definition:
   T(0,0) = 1,
   T(n,k) = T(n‑1,k) + T(n‑1,k‑1) + T(n‑2,k) – T(n‑2,k‑2),
   with T(n,k)=0 if n<k or k<0.
   The sequence a is obtained by concatenating the rows
   0,1,2,…, that is
   a(offset(r)+k) = T(r,k)    where offset(r)=r(r+1)/2.      */

#include <stdlib.h>
#include <stdbool.h>

/* Ensure a buffer can hold at least 'needed' elements */
static void ensure_cap(seq_t **p, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 64 : *cap;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        *p = tmp;
        *cap = newcap;
    }
}

/* Return the n-th term of OEIS sequence A201947 */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq_vals = NULL;   /* flattened sequence values */
    static idx_t seq_len  = 0;        /* number of stored values   */
    static size_t seq_cap = 0;        /* capacity of seq_vals      */

    static seq_t *prev      = NULL;   /* row n‑1                   */
    static seq_t *prev_prev = NULL;   /* row n‑2                   */
    static size_t  prev_len      = 0; /* length of row n‑1         */
    static size_t  prev_prev_len = 0; /* length of row n‑2         */
    static idx_t   cur_row       = 0; /* last computed row index   */

    /* Initialise with row 0. */
    if (!initialized) {
        ensure_cap(&seq_vals, &seq_cap, 1);
        seq_vals[0] = (seq_t)1;
        seq_len = 1;

        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        if (!prev) exit(1);
        prev[0] = (seq_t)1;

        prev_prev_len = 0;
        prev_prev = NULL;

        cur_row = 0;
        initialized = true;
    }

    /* Generate rows until we have at least n+1 terms. */
    while (seq_len <= n) {
        idx_t next_row = cur_row + 1;                      /* new row index   */
        size_t cur_len = (size_t)(next_row + 1);           /* number of terms */

        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) exit(1);

        /* Compute the new row from the two previous rows. */
        for (size_t k = 0; k < cur_len; ++k) {
            seq_t t1 = (k < prev_len) ? prev[k] : (seq_t)0;
            seq_t t2 = (k > 0 && k-1 < prev_len) ? prev[k-1] : (seq_t)0;
            seq_t t3 = (k < prev_prev_len) ? prev_prev[k] : (seq_t)0;
            seq_t t4 = (k > 1 && k-2 < prev_prev_len) ? prev_prev[k-2] : (seq_t)0;
            cur[k] = t1 + t2 + t3 - t4;
        }

        /* Append the new row to the flattened sequence. */
        ensure_cap(&seq_vals, &seq_cap, seq_len + cur_len);
        for (size_t i = 0; i < cur_len; ++i)
            seq_vals[seq_len + i] = cur[i];
        seq_len += cur_len;

        /* Prepare for next iteration. */
        if (prev_prev)
            free(prev_prev);
        prev_prev = prev;
        prev_prev_len = prev_len;

        prev = cur;
        prev_len = cur_len;

        cur_row = next_row;
    }

    return seq_vals[n];
}
