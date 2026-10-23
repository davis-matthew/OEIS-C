/*
 * A144493  Triangle read by rows. Let k=-3, m=3. Row n is a vector v(n)
 *          defined as follows:
 *          v(n)=k*{0,v(n-2),0}+m*{-(m-1)/m,v(n-1)}+m*{v(n-1),-(m-1)/m}.
 * https://oeis.org/A144493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows generated from the recurrence of A144493.
   Recurrence (k = -3, m = 3):
     v(0)   = {1}
     v(1)   = {1, 1}
     For n >= 2, let len = n + 1. Then for 0 < i < len-1
       v(n)[i] = 3*v(n-1)[i] + 3*v(n-1)[i-1] - 3*v(n-2)[i-1]
     and
       v(n)[0]       = 3*v(n-1)[0] - 2
       v(n)[len-1]   = 3*v(n-1)[len-2] - 2
   The sequence a(n) is the n‑th element in the concatenation of all rows:
     a(n) = v(r)[n - r(r+1)/2]   where r is the unique integer with
     r(r+1)/2 <= n < (r+1)(r+2)/2. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static idx_t max_row = 0;
static seq_t **rows = NULL;
static size_t rows_cap = 0;

/* Ensure row data up to 'want' is available. */
static void ensure_rows(idx_t want)
{
    if (!rows) {
        rows_cap = 2;
        rows = (seq_t**)calloc(rows_cap, sizeof(seq_t*));
        if (!rows) abort();
        /* Row 0 */
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows[0]) abort();
        rows[0][0] = (seq_t)1;
        /* Row 1 */
        rows[1] = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!rows[1]) abort();
        rows[1][0] = rows[1][1] = (seq_t)1;
        max_row = 1;
    }
    while (max_row < want) {
        idx_t next = max_row + 1;
        idx_t len  = (idx_t)(next + 1);       /* row length */
        seq_t *cur = (seq_t*)malloc(len * sizeof(seq_t));
        if (!cur) abort();

        seq_t *prev1 = rows[next-1];           /* v(n-1)  length len-1 */
        seq_t *prev2 = rows[next-2];           /* v(n-2)  length len-2 */

        for (idx_t i = 0; i < len; ++i) {
            seq_t pos = 0, neg = 0;
            /* negative part */
            if (i == 0 || i == len-1) {
                neg = (seq_t)2;
            } else {
                neg = mul_small(prev2[i-1], 3);   /* 3*v(n-2)[i-1] */
            }
            /* positive part */
            if (i > 0) {
                pos = mul_small(prev1[i-1], 3);   /* 3*v(n-1)[i-1] */
            }
            if (i < len-1) {
                seq_t tmp = mul_small(prev1[i], 3);/* 3*v(n-1)[i]   */
                pos += tmp;
            }
            cur[i] = pos - neg;                 /* unsigned subtraction */
        }

        /* grow rows array if necessary */
        if (next + 1 > rows_cap) {
            size_t new_cap = rows_cap * 2;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            if (!tmp) abort();
            rows = tmp;
            rows_cap = new_cap;
        }
        rows[next] = cur;
        max_row = next;
    }
}

/* Return the n-th term of sequence A144493. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t r = 0;
    while ((idx_t)((r + 1) * (r + 2) / 2) <= n) {
        r++;
    }

    /* Compute all required rows */
    ensure_rows(r);

    /* Position within the row */
    idx_t offset = (idx_t)(r * (r + 1) / 2);
    idx_t pos_in_row = n - offset;

    return rows[r][pos_in_row];
}
