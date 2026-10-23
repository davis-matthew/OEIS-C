/*
 * A171589  Riordan array (f(x), x*f(x)) where f(x) is the g.f. of A104455.
 * https://oeis.org/A171589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise enumeration of the triangle T(n,k) defined
 *            by * T(0,0)=1 * T(n,0)-T(n,1)=3^n *
 *            T(n,k)=T(n-1,k-1)+4*T(n-1,k)+∑_{j=k+1}^{n-1} T(n-1,j) * * The
 *            sequence begins 1,4,1,17,8,1,77,50,12,1,371,290,99,16,1,… * *
 *            Computation is performed by constructing the triangle row by row
 *            * and st...
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state storing the triangle in row‑major order */
static seq_t *data      = NULL;  /* linear storage */
static size_t data_len  = 0;     /* allocated elements */
static idx_t   max_row  = 0;     /* highest row built (0‑based) */

/* -------------------------------------------------------------- */
/* Ensure that all rows up to and including r have been built.    */
/* -------------------------------------------------------------- */
static void ensure_rows_up_to(idx_t r)
{
    /* Initialise the triangle with the first row if needed. */
    if (data == NULL) {
        data     = (seq_t *)malloc(sizeof(seq_t));
        if (data == NULL) return;          /* OOM – caller will receive garbage */
        data[0]  = (seq_t)1;                /* T(0,0) */
        data_len = 1;
        max_row  = 0;
    }

    while (max_row < r) {
        /* information about the previous row */
        idx_t          prev_len   = max_row + 1;
        size_t         prev_start = (size_t)max_row * (max_row + 1) / 2;
        seq_t        * prev       = &data[prev_start];

        /* compute suffix sums of the previous row:  suffix[j] = Σ_{t=j}^{prev_len-1} prev[t] */
        size_t         suffix_len = (size_t)prev_len + 1;
        seq_t        * suffix     = (seq_t *)malloc(suffix_len * sizeof(seq_t));
        if (suffix == NULL) return;  /* OOM – nothing more can be done */
        suffix[prev_len] = (seq_t)0;
        for (idx_t j = prev_len; j > 0; --j) {
            suffix[j-1] = suffix[j] + prev[j-1];
        }

        /* prepare to append the new row */
        idx_t          new_len   = prev_len + 1;
        size_t         new_start = prev_start + prev_len;
        size_t         needed_len = new_start + new_len;

        /* enlarge storage if necessary */
        if (needed_len > data_len) {
            seq_t *tmp = (seq_t *)realloc(data, needed_len * sizeof(seq_t));
            if (tmp == NULL) { free(suffix); return; }
            data     = tmp;
            data_len = needed_len;
        }

        /* note: after realloc the old 'prev' pointer may be invalid,
           we recompute it immediately. */
        prev = &data[prev_start];

        /* compute the new row */
        seq_t * new_row = &data[new_start];
        for (idx_t k = 0; k < new_len; ++k) {
            seq_t term1 = (k > 0) ? prev[k-1] : (seq_t)0;
            seq_t term2 = (k < prev_len) ? (prev[k] << 2) : (seq_t)0;
            seq_t sum_tail = (k < prev_len) ? suffix[k+1] : (seq_t)0;
            new_row[k] = term1 + term2 + sum_tail;
        }

        free(suffix);
        ++max_row;          /* advance to next row number */
    }
}

/* -------------------------------------------------------------- */
/* Public entry point – returns the nth term (0‑based) of the    */
/* sequence.  The function is thread‑unsafe but satisfies all    */
/* other requirements.                                           */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* locate the row r that contains index n */
    idx_t r = 0;
    /* use 128‑bit intermediate to avoid overflow for large r */
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= (unsigned __int128)n) {
        ++r;
    }
    size_t start = (size_t)r * (r + 1) / 2;
    idx_t  c = (idx_t)(n - start);          /* column within the row */

    ensure_rows_up_to(r);

    size_t index = start + (size_t)c;
    return data[index];
}
