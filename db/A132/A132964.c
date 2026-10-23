/*
 * A132964  Convolution triangle of A006190.
 * https://oeis.org/A132964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the n‑th term of OEIS sequence A132964.  
   The sequence is the flattened triangle defined by the recurrence  
   T(n,k) = 3*T(n-1,k) + T(n-1,k-1) + T(n-2,k),   T(0,0)=1,  
   and a(n) = T(r,c) where r is the row number and c is the column
   that appears when the triangle is read row‑by‑row. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* flattened sequence */
static size_t seq_cap = 0, seq_len = 0;

static seq_t *row_prev = NULL;     /* row n-1 */
static idx_t  row_prev_len = 0;
static seq_t *row_prev2 = NULL;    /* row n-2 */
static idx_t  row_prev2_len = 0;
static idx_t  next_row = 0;        /* next row to compute */

/* Build the triangle up to enough rows to cover index n. */
static void ensure_up_to(idx_t n)
{
    while (seq_len <= (size_t)n) {
        idx_t i = next_row;                 /* current row index */
        size_t len = (size_t)(i + 1);       /* row length */
        seq_t *row_cur = (seq_t*)calloc(len, sizeof(seq_t));
        if (!row_cur) abort();

        if (i == 0) {
            /* T(0,0) = 1 */
            row_cur[0] = 1;
        } else {
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (i > 0 && k < row_prev_len) {
                    val += (seq_t)3 * row_prev[(size_t)k];
                }
                if (k > 0 && i > 0 && k - 1 < row_prev_len) {
                    val += row_prev[(size_t)(k - 1)];
                }
                if (i >= 2 && k < row_prev2_len) {
                    val += row_prev2[(size_t)k];
                }
                row_cur[(size_t)k] = val;
            }
        }

        /* append current row to the flattened sequence */
        for (size_t j = 0; j < len; ++j) {
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(seq,
                                             new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = row_cur[j];
        }

        /* rotate row buffers */
        free(row_prev2);
        row_prev2 = row_prev;
        row_prev2_len = row_prev_len;
        row_prev = row_cur;
        row_prev_len = (idx_t)len;
        ++next_row;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return seq[(size_t)n];
}
